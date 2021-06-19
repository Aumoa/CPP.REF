// Copyright 2020-2021 Aumoa.lib. All right reserved.

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
		LogSystem::Log(LogCamera, Error, L"There is no cached camera component. Please call CachePlayerCamera with PlayerController instance to binding current camera component that owned by possessed pawn.");
		_cachedView = MinimalViewInfo();
	}

	_cachedView = _cachedBindCamera->GetViewInfo(elapsedTime);
}

void APlayerCameraManager::CachePlayerCamera(APlayerController* controller)
{
	_cachedBindCamera = controller->FindPlayerCameraComponent();
}

MinimalViewInfo APlayerCameraManager::GetCachedCameraView() const
{
	return _cachedView;
}