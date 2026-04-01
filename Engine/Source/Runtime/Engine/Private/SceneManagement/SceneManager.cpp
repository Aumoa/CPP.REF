// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "SceneManagement/SceneManager.h"
#include "SceneManagement/Scene.h"
#include "Rendering/RenderScene.h"

namespace Ayla
{
	SceneManager::SceneManager(GameInstance* gameInstance)
		: m_GameInstance(gameInstance)
		, m_RenderScene(std::make_shared<RenderScene>())
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
			m_ActiveScene->Activate(this);
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

	void SceneManager::MarkRendererDirty(Renderer* renderer)
	{
		m_DirtyRenderers.insert(renderer);
	}

	void SceneManager::UnmarkRendererDirty(Renderer* renderer)
	{
		m_DirtyRenderers.erase(renderer);
	}
}