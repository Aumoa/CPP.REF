// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Diagnostics/PerformanceTimer.h"

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