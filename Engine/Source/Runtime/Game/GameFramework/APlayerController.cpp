// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;
import SC.Runtime.Game;

using namespace std;
using namespace std::chrono;

APlayerController::APlayerController() : Super()
{
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