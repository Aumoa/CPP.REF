// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "SceneManagement/SceneManager.h"
#include "SceneManagement/Scene.h"

namespace Ayla
{
	RPtr<Scene> SceneManager::s_ActiveScene;
	std::vector<RPtr<Scene>> SceneManager::s_AdditiveScenes;

	Task<> SceneManager::LoadSceneAsync(RPtr<Scene> scene, LoadSceneMode mode, CancellationToken cancellationToken)
	{
		if (s_ActiveScene)
		{
			s_ActiveScene->Destroy();
		}

		s_ActiveScene = scene;

		if (s_ActiveScene)
		{
			s_ActiveScene->Activate();
		}

		return Task<>::CompletedTask();
	}
}