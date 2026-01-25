// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GameContext.h"
#include "GameInstance.h"
#include "Engine.h"
#include "SceneManagement/SceneManager.h"

namespace Ayla
{
	thread_local Engine* m_Engine;
	thread_local GameInstance* m_GameInstance;
	thread_local SceneManager* m_SceneManager;

	void GameContext::BeginContext(Engine* engine, GameInstance* gameInstance, SceneManager* sceneManager)
	{
		m_Engine = engine;
		m_GameInstance = gameInstance;
		m_SceneManager = sceneManager;
	}

	void GameContext::EndContext()
	{
		m_Engine = nullptr;
		m_GameInstance = nullptr;
		m_SceneManager = nullptr;
	}

	Engine* GameContext::GetEngine()
	{
		return m_Engine;
	}

	GameInstance* GameContext::GetGameInstance()
	{
		return m_GameInstance;
	}

	SceneManager* GameContext::GetSceneManager()
	{
		return m_SceneManager;
	}
}