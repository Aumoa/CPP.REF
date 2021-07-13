// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "PlayerCameraManager.h"
#include "GameStructures.h"
#include "LogGame.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

using namespace std::chrono;

using enum ELogVerbosity;

APlayerCameraManager::APlayerCameraManager()
{
}

void APlayerCameraManager::UpdateCamera(duration<float> elapsedTime)
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

void APlayerCameraManager::CachePlayerCamera(APlayerController* controller)
{
	_cachedBindCamera = controller->FindPlayerCameraComponent();
}

MinimalViewInfo APlayerCameraManager::GetCachedCameraView() const
{
	return _cachedView;
}