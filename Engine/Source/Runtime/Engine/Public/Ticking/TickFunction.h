// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ticking/TickTiming.h"

namespace Ayla
{
	class ScriptableBehavior;

	struct ENGINE_API TickFunction
	{
		ScriptableBehavior* Owner = nullptr;
		bool bCanEverTick : 1 = false;
		bool bEnabled : 1 = false;
		TickTiming Timing = TickTiming::Update;
		size_t TickIndex = std::numeric_limits<size_t>::max();

		void ExecuteTick(TickTiming timing, const TimeSpan& deltaTime);
	};
}