// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Scripting/ScriptingSceneManager.h"
#include "GameContext.h"
#include "SceneManagement/SceneManager.h"
#include "SceneManagement/Scene.h"

namespace Ayla
{
	void ScriptingSceneManager::LoadScene(SharedPtr<Scene> scene, LoadSceneMode mode)
	{
		auto sceneManager = GameContext::GetSceneManager();
		sceneManager->LoadScene(scene, mode);
	}
}