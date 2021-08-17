// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameInstance.h"
#include "GameEngine.h"
#include "IFrameworkView.h"
#include "LogGame.h"
#include "Components/InputComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Level/World.h"

APlayerController::APlayerController() : Super()
{
	_inputComponent = CreateSubobject<InputComponent>();
	AddOwnedComponent(_inputComponent);
}

CameraComponent* APlayerController::FindPlayerCameraComponent() const
{
	APawn* pawn = GetPawn();
	if (pawn == nullptr)
	{
		return nullptr;
	}

	return pawn->GetComponentAs<CameraComponent>();
}

void APlayerController::SpawnCameraManager(World* level)
{
	_cameraManager = level->SpawnActor<APlayerCameraManager>();
}

void APlayerController::UpdateCameraManager(std::chrono::duration<float> elapsedTime)
{
	_cameraManager->CachePlayerCamera(this);
	_cameraManager->UpdateCamera(elapsedTime);
}

Ray<3> APlayerController::ScreenPointToRay(int32 screenX, int32 screenY) const
{
	if (!ensureMsgf(_cameraManager != nullptr, L"PlayerCameraManager does not registered."))
	{
		return {};
	}

	MinimalViewInfo view = _cameraManager->GetCachedCameraView();
	if (view.bInit == false)
	{
		SE_LOG(LogPlayerController, Verbose, L"Player camera does not initialized. Abort.");
		return {};
	}

	World* world = GetWorld();
	GameEngine* engine = world->GetEngine();
	GameInstance* gameInstance = engine->GetGameInstance();
	IFrameworkView* frameworkView = gameInstance->GetFrameworkView();

	int32 frameworkWidth = frameworkView->GetFrameworkWidth();
	int32 frameworkHeight = frameworkView->GetFrameworkHeight();

	float aspectRatio = (float)frameworkWidth / frameworkHeight;

	Vector3 up = Vector3(0, 1, 0);
	Vector3 forward = Vector3(0, 0, 1);

	auto vm = Matrix4x4::LookToLH(view.Location, view.Rotation.RotateVector(forward), view.Rotation.RotateVector(up));
	auto pm = Matrix4x4::PerspectiveFovLH(view.FOVAngle.ToRadians(), aspectRatio, view.NearPlane, view.FarPlane);
	auto vminv = vm.GetInverse();

	float vx = (float)screenX / frameworkWidth;
	float vy = (float)screenY / frameworkHeight;

	Vector3 viewSpace;
	viewSpace.X() = (vx * 2.0f - 1.0f - pm.V[2][0]) / pm.V[0][0];
	viewSpace.Y() = (vy * -2.0f + 1.0f - pm.V[2][1]) / pm.V[1][1];
	viewSpace.Z() = 1.0;

	Vector3 dir;
	Vector3 org;

	dir.X() = viewSpace.X() * vminv.V[0][0] + viewSpace.Y() * vminv.V[1][0] + viewSpace.Z() * vminv.V[2][0];
	dir.Y() = viewSpace.X() * vminv.V[0][1] + viewSpace.Y() * vminv.V[1][1] + viewSpace.Z() * vminv.V[2][1];
	dir.Z() = viewSpace.X() * vminv.V[0][2] + viewSpace.Y() * vminv.V[1][2] + viewSpace.Z() * vminv.V[2][2];

	org.X() = vminv.V[3][0];
	org.Y() = vminv.V[3][1];
	org.Z() = vminv.V[3][2];

	Ray<3> ray;
	ray.Origin = org;
	ray.Direction = dir.GetNormal();
	ray.Distance = view.FarPlane;

	return ray;
}