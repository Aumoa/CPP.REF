// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "PlayerController.h"
#include "Pawn.h"
#include "Components/InputComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Level/World.h"

using namespace std;
using namespace std::chrono;

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

void APlayerController::UpdateCameraManager(duration<float> elapsedTime)
{
	_cameraManager->CachePlayerCamera(this);
	_cameraManager->UpdateCamera(elapsedTime);
}