// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Camera/PlayerCameraManager.h"
#include "LogGame.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

APlayerCameraManager::APlayerCameraManager()
{
}

void APlayerCameraManager::UpdateCamera(float elapsedTime)
{
	if (_cachedBindCamera == nullptr)
	{
		_cachedView.bInit = true;
		_cachedView.Location = GetActorLocation();
		_cachedView.Rotation = GetActorRotation();
		_cachedView.NearPlane = 0.1f;
		_cachedView.FarPlane = 1000.0f;
		_cachedView.FOVAngle = 45.0f;
	}
	else
	{
		_cachedView = _cachedBindCamera->GetViewInfo(elapsedTime);
	}
}

void APlayerCameraManager::InitializeFor(APlayerController* InController)
{
	_InitializedController = InController;
}

APlayerController* APlayerCameraManager::GetController()
{
	return _InitializedController;
}

void APlayerCameraManager::CachePlayerCamera(APlayerController* controller)
{
	_cachedBindCamera = controller->FindPlayerCameraComponent();
}

MinimalViewInfo APlayerCameraManager::GetCachedCameraView()
{
	return _cachedView;
}