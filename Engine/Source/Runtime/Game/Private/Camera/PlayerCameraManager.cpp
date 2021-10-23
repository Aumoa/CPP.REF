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
		_CachedView.bInit = true;
		_CachedView.Location = GetActorLocation();
		_CachedView.Rotation = GetActorRotation();
		_CachedView.NearPlane = 0.1f;
		_CachedView.FarPlane = 1000.0f;
		_CachedView.FOVAngle = 45.0f;
	}
	else
	{
		_CachedView = _cachedBindCamera->GetViewInfo(elapsedTime);
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
	return _CachedView;
}