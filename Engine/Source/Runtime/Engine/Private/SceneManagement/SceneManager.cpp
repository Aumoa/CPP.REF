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

	void SceneManager::UnloadAllScenes()
	{
		for (auto& additiveScene : m_AdditiveScenes)
		{
			additiveScene->Destroy();
		}

		m_AdditiveScenes.clear();

		if (m_ActiveScene)
		{
			m_ActiveScene->Destroy();
		}
	}

	void SceneManager::GetAllCameraComponents(std::vector<Camera*>* output) const
	{
		if (m_ActiveScene)
		{
			auto cameras = m_ActiveScene->GetCameraComponents();
			output->insert(output->end(), cameras.begin(), cameras.end());
		}

		for (auto& scene : m_AdditiveScenes)
		{
			auto cameras = scene->GetCameraComponents();
			output->insert(output->end(), cameras.begin(), cameras.end());
		}
	}

	void SceneManager::DispatchTick(TickTiming timing)
	{
		m_ActiveScene->DispatchTick(timing);
	}
}