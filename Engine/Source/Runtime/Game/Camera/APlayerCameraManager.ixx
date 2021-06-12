// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:APlayerCameraManager;

import std.core;
import :AActor;
import :MinimalViewInfo;

using namespace std::chrono;

export class CameraComponent;
export class APlayerController;

export class APlayerCameraManager : public AActor
{
public:
	using Super = AActor;

private:
	CameraComponent* _cachedBindCamera = nullptr;

public:
	APlayerCameraManager();

	virtual MinimalViewInfo UpdateCamera(duration<float> elapsedTime) const;

	void CachePlayerCamera(APlayerController* controller);
};