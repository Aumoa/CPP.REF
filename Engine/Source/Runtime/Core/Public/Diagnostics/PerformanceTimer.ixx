// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.Diagnostics:PerformanceTimer;

export import Core.System;

export class CORE_API PerformanceTimer
{
public:
	PerformanceTimer() noexcept;
	PerformanceTimer(const PerformanceTimer& Rhs) noexcept;
	~PerformanceTimer() noexcept;

	void Start() noexcept;
	void Stop() noexcept;

	void Reset() noexcept;
	void Restart() noexcept;

	bool IsRunning() const noexcept;
	TimeSpan GetElapsed() const noexcept;

	static PerformanceTimer StartNew() noexcept;

public:
	PerformanceTimer& operator =(const PerformanceTimer& Rhs) noexcept;

private:
	std::optional<DateTime> StartTime;
	TimeSpan Elapsed;
};