// Copyright 2020 Aumoa.lib. All right reserved.

#include "Components/PlayerCameraManager.h"

#include "Engine.h"
#include "GameViewport.h"
#include "Logging/LogMacros.h"
#include "Framework/PlayerController.h"
#include "Framework/Pawn.h"
#include "Components/CameraComponent.h"
#include "SceneRendering/MinimalViewInfo.h"

PlayerCameraManager::PlayerCameraManager() : Super()
	, bPrintNoCameraWarning(true)
	, pawnCamera(nullptr)
	, lastUpdatedTransform(Transform::Identity)
{

}

PlayerCameraManager::~PlayerCameraManager()
{

}

void PlayerCameraManager::CalcCameraView(MinimalViewInfo& outViewInfo) const
{
	static constexpr const float DefaultFOV = 0.25f * 3.14f;

	if (pawnCamera != nullptr)
	{
		bPrintNoCameraWarning = true;
		pawnCamera->CalcCameraView(outViewInfo);
	}
	else
	{
		APlayerController* playerController = GetOwner<APlayerController>();
		if (playerController == nullptr)
		{
			SE_LOG(LogCamera, Error, L"PlayerCameraManager component is not attached to player controller. Abort.");
			return;
		}

		Transform updateTransform;

		APawn* pawn = playerController->GetPawn();
		if (pawn == nullptr)
		{
			PrintNoCameraWarning(L"Camera component is not setted and there is no possessed pawn. Using last updated transform.");
			updateTransform = lastUpdatedTransform;
		}
		else
		{
			PrintNoCameraWarning(L"Camera component is not setted. Calc camera view using default transform of possessed pawn.");
			updateTransform = pawn->GetActorTransform();
		}

		GameViewport* viewport = GEngine.GetGameViewport();

		outViewInfo.FOV = DefaultFOV;
		outViewInfo.AspectRatio = (float)viewport->ResolutionX / (float)viewport->ResolutionY;
		outViewInfo.Location = updateTransform.Translation;
		outViewInfo.Rotation = updateTransform.Rotation;
		outViewInfo.Apply();
	}
}

void PlayerCameraManager::UpdateCameraComponent()
{
	pawnCamera = nullptr;

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

	pawnCamera = camera;
}

void PlayerCameraManager::PrintNoCameraWarning(TRefPtr<String> message) const
{
	if (bPrintNoCameraWarning)
	{
		SE_LOG(LogCamera, Warning, message);
		bPrintNoCameraWarning = false;
	}
}