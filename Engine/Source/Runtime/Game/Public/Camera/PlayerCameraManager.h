// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructures.h"

class CameraComponent;
class APlayerController;

class GAME_API APlayerCameraManager : public AActor
{
	CLASS_BODY(APlayerCameraManager)

private:
	CameraComponent* _cachedBindCamera = nullptr;
	MinimalViewInfo _cachedView;

public:
	APlayerCameraManager();

	virtual void UpdateCamera(std::chrono::duration<float> elapsedTime);

	void CachePlayerCamera(APlayerController* controller);
	MinimalViewInfo GetCachedCameraView() const;
};