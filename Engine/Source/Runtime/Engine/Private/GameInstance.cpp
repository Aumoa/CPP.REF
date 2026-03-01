// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GameInstance.h"
#include "Engine.h"
#include "GameContext.h"
#include "SceneManagement/Scene.h"
#include "SceneManagement/SceneManager.h"
#include "Threading/MainSynchronizationContext.h"

namespace Ayla
{
	GameInstance::GameInstance()
	{
	}

	GameInstance::~GameInstance() noexcept
	{
	}

	Task<> GameInstance::InitializeAsync(std::stop_token cancellationToken)
	{
		m_SyncContext = std::make_shared<MainSynchronizationContext>();
		SynchronizationContext::SetSynchronizationContext(m_SyncContext);
		try__
		{
			m_SceneManager = std::make_shared<SceneManager>();

			GameContext::BeginContext(m_Engine, this, m_SceneManager.get());
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

		co_return;
	}

	void GameInstance::Tick(TickTiming timing, const TimeSpan& deltaTime)
	{
		SynchronizationContext::SetSynchronizationContext(m_SyncContext);
		try__
		{
			GameContext::BeginContext(m_Engine, this, m_SceneManager.get());
			m_SyncContext->Tick();
			m_SceneManager->DispatchTick(timing);
		}
		finally__
		{
			SynchronizationContext::SetSynchronizationContext(nullptr);
			GameContext::EndContext();
		}
		end_try__;
	}

	SharedPtr<Engine> GameInstance::GetEngine()
	{
		return m_Engine->AsShared();
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