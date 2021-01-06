// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/PlayerCameraManager.h"

#include "Logging/EngineLogCategory.h"
#include "Logging/LogMacros.h"
#include "Framework/PlayerController.h"
#include "Framework/Pawn.h"
#include "Components/CameraComponent.h"

PlayerCameraManager::PlayerCameraManager() : Super()
	, pawnCamera(nullptr)
{

}

PlayerCameraManager::~PlayerCameraManager()
{

}

void PlayerCameraManager::CalcCameraView(MinimalViewInfo& outViewInfo) const
{

}

void PlayerCameraManager::UpdateCameraComponent()
{
	APlayerController* playerController = GetOwner<APlayerController>();
	if (playerController == nullptr)
	{
		SE_LOG(LogCamera, Error, L"Component owner is not a player controller. Abort.");
		return;
	}

	APawn* pawn = playerController->GetPawn();
	if (pawn == nullptr)
	{
		SE_LOG(LogCamera, Error, L"Owner player controller is not possessed to any pawn. Abort.");
		return;
	}

	CameraComponent* camera = pawn->GetComponent<CameraComponent>();
	if (camera == nullptr)
	{
		SE_LOG(LogCamera, Verbose, L"Possessed pawn have not a camera component. Using default camera view property.");
		return;
	}
}