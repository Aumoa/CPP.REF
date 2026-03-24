// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ticking/TickTiming.h"

namespace Ayla
{
	struct TickFunction;

	class ENGINE_API TickManager
	{
	private:
		std::vector<TickFunction*> m_TickFunctions[(size_t)TickTiming::Count];

	public:
		void Tick(TickTiming timing, const TimeSpan& deltaTime);

		void AddScriptableBehavior(TickFunction* tickFunction);
		void RemoveScriptableBehavior(TickFunction* tickFunction);
	};
}
