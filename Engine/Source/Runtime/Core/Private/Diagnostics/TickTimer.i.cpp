// Copyright 2020-2023 Aumoa.lib. All right reserved.

import Core;

TickTimer::TickTimer()
{
}

TickTimer::~TickTimer() noexcept
{
}

TimeSpan TickTimer::Tick() noexcept
{
	if (Timer.IsRunning() == false)
	{
		// Start timer when first tick is executing.
		Timer.Start();
		LastDur = TimeSpan::FromSeconds(0);
	}

	TimeSpan Cur = Timer.GetElapsed();
	TimeSpan Del = Cur - LastDur;
	LastDur = Cur;
	return Del;
}