// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class Engine;
	class GameInstance;
	class SceneManager;

	class ENGINE_API GameContext : public Object
	{
	public:
		static void BeginContext(Engine* engine, GameInstance* gameInstance, SceneManager* sceneManager);
		static void EndContext();

		static Engine* GetEngine();
		static GameInstance* GetGameInstance();
		static SceneManager* GetSceneManager();
	};
}