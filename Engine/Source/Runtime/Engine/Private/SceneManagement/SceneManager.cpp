// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "SceneManagement/SceneManager.h"
#include "SceneManagement/Scene.h"

namespace Ayla
{
	SceneManager::SceneManager()
	{
	}

	SceneManager::~SceneManager() noexcept
	{
	}

	void SceneManager::LoadScene(SharedPtr<Scene> scene, LoadSceneMode mode)
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
	}
}