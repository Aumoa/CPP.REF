// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GameInstance.h"
#include "Engine.h"
#include "GameContext.h"
#include "SceneManagement/Scene.h"
#include "SceneManagement/SceneManager.h"
#include "Threading/MainSynchronizationContext.h"
#include "Ticking/TickManager.h"
#include "InputSystem/InputManager.h"
#include "GenericWindow.h"

namespace Ayla
{
	GameInstance::GameInstance()
	{
		m_TickManager = std::make_unique<TickManager>();
		m_InputManager = New<InputManager>();
	}

	GameInstance::~GameInstance() noexcept
	{
	}

	void GameInstance::Initialize(GenericWindow* window)
	{
		m_SyncContext = std::make_shared<MainSynchronizationContext>();
		SynchronizationContext::SetSynchronizationContext(m_SyncContext);
		try__
		{
			window->AddExtension(m_InputManager);
			m_SceneManager = std::make_shared<SceneManager>(this);

			GameContext::BeginContext(m_Engine, this);
			ScriptingInitialize(InitializeTiming::BeforeSceneLoad);
			m_SceneManager->LoadScene(GetEntryScene());
			ScriptingInitialize(InitializeTiming::AfterSceneLoad);
		}
		finally__
		{
			GameContext::EndContext();
			SynchronizationContext::SetSynchronizationContext(nullptr);
		}
		end_try__;
	}

	void GameInstance::Tick(TickTiming timing, const TimeSpan& deltaTime)
	{
		SynchronizationContext::SetSynchronizationContext(m_SyncContext);
		try__
		{
			GameContext::BeginContext(m_Engine, this);
			m_SyncContext->Tick();
			m_TickManager->Tick(timing, deltaTime);
		}
		finally__
		{
			SynchronizationContext::SetSynchronizationContext(nullptr);
			GameContext::EndContext();
		}
		end_try__;
	}

	void GameInstance::Shutdown()
	{
		m_SceneManager->UnloadAllScenes();
	}

	SharedPtr<Engine> GameInstance::GetEngine()
	{
		return m_Engine->AsShared<Engine>();
	}

	SharedPtr<Scene> GameInstance::GetEntryScene_Implementation()
	{
		return m_EntryScene ? m_EntryScene : New<Scene>();
	}

	void GameInstance::InternalSetEngine(SharedPtr<Engine> engine)
	{
		m_Engine = engine.Get();
	}

	void GameInstance::ScriptingInitialize_Implementation(InitializeTiming timing)
	{
	}
}
