// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Components/ScriptableBehavior.h"
#include "Actors/GameObject.h"
#include "SceneManagement/Scene.h"
#include "SceneManagement/SceneManager.h"
#include "Ticking/TickManager.h"
#include "GameInstance.h"

namespace Ayla
{
	ScriptableBehavior::ScriptableBehavior()
		: Super()
		, m_PrimaryTick{ .Owner = this, .Timing = TickTiming::Update }
	{
	}

	ScriptableBehavior::~ScriptableBehavior() noexcept
	{
		check(m_PrimaryTick.TickIndex == std::numeric_limits<size_t>::max());
	}

	void ScriptableBehavior::Awake()
	{
		m_bDidAwake = true;
		if (m_PrimaryTick.bCanEverTick)
		{
			auto* tm = GetGameObject()->GetScene()->GetSceneManager()->GetGameInstance()->GetTickManager();
			tm->AddScriptableBehavior(&m_PrimaryTick);
		}
	}

	void ScriptableBehavior::OnDestroy()
	{
		if (!m_bDidAwake)
		{
			return;
		}

		if (m_PrimaryTick.bCanEverTick)
		{
			auto* tm = GetGameObject()->GetScene()->GetSceneManager()->GetGameInstance()->GetTickManager();
			tm->RemoveScriptableBehavior(&m_PrimaryTick);
		}
	}

	void ScriptableBehavior::OnEnable()
	{
		m_PrimaryTick.bEnabled = true;
	}

	void ScriptableBehavior::OnDisable()
	{
		m_PrimaryTick.bEnabled = false;
	}
}