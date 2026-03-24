// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Behavior.h"
#include "Ticking/TickFunction.h"
#include "ScriptableBehavior.gen.h"

namespace Ayla
{
	enum class TickTiming;
	class TickManager;

	ACLASS()
	class ENGINE_API ScriptableBehavior : public Behavior
	{
		GENERATED_BODY()
		friend class TickManager;

	private:
		bool m_bDidAwake : 1 = false;

	protected:
		TickFunction m_PrimaryTick;

	public:
		ScriptableBehavior();
		virtual ~ScriptableBehavior() noexcept override;

		virtual void Awake();
		virtual void OnDestroy();
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void Tick(TickTiming timing, const TimeSpan& deltaTime) = 0;

		bool DidAwake() const noexcept { return m_bDidAwake; }
	};
}