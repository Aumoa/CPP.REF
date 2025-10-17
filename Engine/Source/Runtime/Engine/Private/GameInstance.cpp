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

	SharedPtr<Scene> GameInstance::GetEntryScene_Implementation()
	{
		return m_EntryScene ? m_EntryScene : New<Scene>();
	}
}