// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GameInstance.h"
#include "Engine.h"
#include "SceneManagement/Scene.h"

namespace Ayla
{
	GameInstance::GameInstance()
	{
	}

	GameInstance::~GameInstance() noexcept
	{
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
}