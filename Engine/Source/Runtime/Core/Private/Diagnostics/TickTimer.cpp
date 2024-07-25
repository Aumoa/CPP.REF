// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:TickTimer;

export import :PerformanceTimer;
export import :TimeSpan;

export class CORE_API TickTimer
{
private:
	PerformanceTimer Timer;
	TimeSpan LastDur;

public:
	TickTimer();
	~TickTimer() noexcept;

	TimeSpan Tick() noexcept;
};