// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Ticking/TickManager.h"
#include "Ticking/TickFunction.h"

namespace Ayla
{
	void TickManager::Tick(TickTiming timing, const TimeSpan& deltaTime)
	{
		for (auto& tickFunction : m_TickFunctions[(size_t)timing])
		{
			tickFunction->ExecuteTick(timing, deltaTime);
		}
	}

	void TickManager::AddScriptableBehavior(TickFunction* tickFunction)
	{
		check(tickFunction->TickIndex == (size_t)-1);
		auto& list = m_TickFunctions[(size_t)tickFunction->Timing];
		size_t tickIndex = list.size();
		list.emplace_back(tickFunction);
		tickFunction->TickIndex = tickIndex;
	}

	void TickManager::RemoveScriptableBehavior(TickFunction* tickFunction)
	{
		size_t tickIndex = tickFunction->TickIndex;
		check(tickIndex != (size_t)-1);
		auto& list = m_TickFunctions[(size_t)tickFunction->Timing];
		size_t lastIndex = list.size() - 1;
		if (tickIndex != lastIndex)
		{
			std::swap(list[tickIndex], list[lastIndex]);
			list[tickIndex]->TickIndex = tickIndex;
		}
		list.pop_back();
		tickFunction->TickIndex = (size_t)-1;
	}
}