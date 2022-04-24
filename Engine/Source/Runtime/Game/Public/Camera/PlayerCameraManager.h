// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "MinimalViewInfo.h"

namespace libty::inline Game
{
	class SCameraComponent;
	class APlayerController;

	class GAME_API APlayerCameraManager : public AActor
	{
		GENERATED_BODY(APlayerCameraManager)

	private:
		APlayerController* _InitializedController = nullptr;
		SCameraComponent* _cachedBindCamera = nullptr;
		MinimalViewInfo _CachedView;

	public:
		APlayerCameraManager();

		virtual void UpdateCamera(float elapsedTime);

		void InitializeFor(APlayerController* InController);
		APlayerController* GetController();

		void CachePlayerCamera(APlayerController* controller);
		MinimalViewInfo GetCachedCameraView();
	};
}