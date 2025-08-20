// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GameInstance.h"
#include "SceneManagement/Scene.h"

namespace Ayla
{
	GameInstance::GameInstance()
	{
	}

	GameInstance::~GameInstance() noexcept
	{
	}

	RPtr<Scene> GameInstance::GetEntryScene()
	{
		return m_EntryScene ? (RPtr<Scene>)m_EntryScene : New<Scene>();
	}
}