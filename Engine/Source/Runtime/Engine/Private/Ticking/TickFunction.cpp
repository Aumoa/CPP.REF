// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Ticking/TickFunction.h"
#include "Components/ScriptableBehavior.h"

namespace Ayla
{
	void TickFunction::ExecuteTick(TickTiming timing, const TimeSpan& deltaTime)
	{
		check(bCanEverTick && bEnabled);
		Owner->Tick(timing, deltaTime);
	}
}