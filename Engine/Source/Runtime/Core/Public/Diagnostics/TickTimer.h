// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Diagnostics/PerformanceTimer.h"
#include "TimeSpan.h"

namespace Ayla
{
	class CORE_API TickTimer
	{
	private:
		PerformanceTimer Timer;
		TimeSpan LastDur;

	public:
		TickTimer();
		~TickTimer() noexcept;

		TimeSpan Tick() noexcept;
	};
}