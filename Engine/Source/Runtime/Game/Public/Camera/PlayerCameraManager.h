// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinimalViewInfo.h"

class SCameraComponent;
class APlayerController;

class GAME_API APlayerCameraManager : public AActor
{
	GENERATED_BODY(APlayerCameraManager)

private:
	APlayerController* _InitializedController = nullptr;
	SCameraComponent* _cachedBindCamera = nullptr;
	MinimalViewInfo _cachedView;

public:
	APlayerCameraManager();

	virtual void UpdateCamera(float elapsedTime);

	void InitializeFor(APlayerController* InController);
	APlayerController* GetController();

	void CachePlayerCamera(APlayerController* controller);
	MinimalViewInfo GetCachedCameraView();
};