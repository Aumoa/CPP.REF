// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameInstance.h"
#include "GameEngine.h"
#include "LogGame.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Level/World.h"
#include "GameFramework/LocalPlayer.h"
#include "Components/SceneComponent.h"

using namespace ::libty;

APlayerController::APlayerController() : Super()
	, PlayerCameraManagerClass(APlayerCameraManager::TypeId)
{
}

SCameraComponent* APlayerController::FindPlayerCameraComponent()
{
	APawn* pawn = GetPawn();
	if (pawn == nullptr)
	{
		return nullptr;
	}

	return pawn->GetComponentAs<SCameraComponent>();
}

void APlayerController::UpdateCameraManager(float elapsedTime)
{
	if (_PlayerCameraManager)
	{
		_PlayerCameraManager->CachePlayerCamera(this);
		_PlayerCameraManager->UpdateCamera(elapsedTime);
	}
}

Ray<3> APlayerController::ScreenPointToRay(int32 screenX, int32 screenY)
{
	throw Exception("NOT IMPLEMENTED");
	//if (!ensureMsgf(_PlayerCameraManager != nullptr, L"PlayerCameraManager does not registered."))
	//{
	//	return {};
	//}

	//MinimalViewInfo view = _PlayerCameraManager->GetCachedCameraView();
	//if (view.bInit == false)
	//{
	//	SE_LOG(LogPlayerController, Verbose, L"Player camera does not initialized. Abort.");
	//	return {};
	//}

	//SWorld* world = GetWorld();
	//auto& frameworkView = IApplicationInterface::Get();

	//const auto [frameworkWidth, frameworkHeight] = frameworkView.GetViewportSize();

	//float aspectRatio = (float)frameworkWidth / frameworkHeight;

	//Vector3 up = Vector3(0, 1, 0);
	//Vector3 forward = Vector3(0, 0, 1);

	//auto vm = Matrix4x4::LookToLH(view.Location, view.Rotation.RotateVector(forward), view.Rotation.RotateVector(up));
	//auto pm = Matrix4x4::PerspectiveFovLH(view.FOVAngle.ToRadians(), aspectRatio, view.NearPlane, view.FarPlane);
	//auto vminv = vm.GetInverse();

	//float vx = (float)screenX / frameworkWidth;
	//float vy = (float)screenY / frameworkHeight;

	//Vector3 viewSpace;
	//viewSpace.X = (vx * 2.0f - 1.0f - pm.V[2][0]) / pm.V[0][0];
	//viewSpace.Y = (vy * -2.0f + 1.0f - pm.V[2][1]) / pm.V[1][1];
	//viewSpace.Z = 1.0;

	//Vector3 dir;
	//Vector3 org;

	//dir.X = viewSpace.X * vminv.V[0][0] + viewSpace.Y * vminv.V[1][0] + viewSpace.Z * vminv.V[2][0];
	//dir.Y = viewSpace.X * vminv.V[0][1] + viewSpace.Y * vminv.V[1][1] + viewSpace.Z * vminv.V[2][1];
	//dir.Z = viewSpace.X * vminv.V[0][2] + viewSpace.Y * vminv.V[1][2] + viewSpace.Z * vminv.V[2][2];

	//org.X = vminv.V[3][0];
	//org.Y = vminv.V[3][1];
	//org.Z = vminv.V[3][2];

	//Ray<3> ray;
	//ray.Origin = org;
	//ray.Direction = dir.GetNormal();
	//ray.Distance = view.FarPlane;

	//return ray;
}

SLocalPlayer* APlayerController::GetLocalPlayer()
{
	return GetWorld()->GetLocalPlayer();
}

APlayerCameraManager* APlayerController::GetPlayerCameraManager()
{
	return _PlayerCameraManager;
}

void APlayerController::PostInitializedComponents()
{
	SpawnPlayerCameraManager();
}

void APlayerController::SpawnPlayerCameraManager()
{
	SWorld* const World = GetWorld();
	checkf(World, L"There is no valid world context.");

	if (_PlayerCameraManager)
	{
		_PlayerCameraManager->DestroyActor();
		_PlayerCameraManager = nullptr;
	}

	_PlayerCameraManager = World->SpawnActor(PlayerCameraManagerClass);
	_PlayerCameraManager->InitializeFor(this);
}