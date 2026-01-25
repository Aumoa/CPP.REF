// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneManagement/LoadSceneMode.h"
#include "ScriptingSceneManager.gen.h"

namespace Ayla
{
	class Scene;

	ACLASS()
	class ENGINE_API ScriptingSceneManager : public Object
	{
		GENERATED_BODY()

	public:
		ScriptingSceneManager() = delete;

		AFUNCTION()
		static void LoadScene(SharedPtr<Scene> scene, LoadSceneMode mode = LoadSceneMode::Single);
	};
}