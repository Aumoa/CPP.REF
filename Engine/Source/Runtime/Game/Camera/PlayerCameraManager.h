// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructures.h"

class CameraComponent;
class APlayerController;

class APlayerCameraManager : public AActor
{
public:
	using Super = AActor;

private:
	CameraComponent* _cachedBindCamera = nullptr;
	MinimalViewInfo _cachedView;

public:
	APlayerCameraManager();

	virtual void UpdateCamera(std::chrono::duration<float> elapsedTime);

	void CachePlayerCamera(APlayerController* controller);
	MinimalViewInfo GetCachedCameraView() const;
};