// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Game;
import SC.Runtime.Core;
import std.core;

using namespace std::chrono;

using static ELogVerbosity;

APlayerCameraManager::APlayerCameraManager()
{
}

MinimalViewInfo APlayerCameraManager::UpdateCamera(duration<float> elapsedTime) const
{
	if (_cachedBindCamera == nullptr)
	{
		LogSystem::Log(LogCamera, Error, L"There is no cached camera component. Please call CachePlayerCamera with PlayerController instance to binding current camera component that owned by possessed pawn.");
		return MinimalViewInfo();
	}

	return _cachedBindCamera->GetViewInfo(elapsedTime);
}

void APlayerCameraManager::CachePlayerCamera(APlayerController* controller)
{
	_cachedBindCamera = controller->FindPlayerCameraComponent();
}