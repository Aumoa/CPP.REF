// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "SceneManagement/SceneManager.h"
#include "SceneManagement/Scene.h"

namespace Ayla
{
	SceneManager::SceneManager()
	{
	}

	Task<void> SceneManager::LoadSceneAsync(std::shared_ptr<Scene> scene, LoadSceneMode mode, CancellationToken cancellationToken)
	{
		if (m_ActiveScene)
		{
			m_ActiveScene->Destroy();
		}

		m_ActiveScene = scene;

		if (m_ActiveScene)
		{
			m_ActiveScene->Activate();
		}

		return Task<>::CompletedTask();
	}
}