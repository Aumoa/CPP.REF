// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DateTime.h"
#include "TimeSpan.h"
#include <optional>

class CORE_API PerformanceTimer
{
public:
	PerformanceTimer() noexcept;
	PerformanceTimer(const PerformanceTimer& rhs) noexcept;
	~PerformanceTimer() noexcept;

	void Start() noexcept;
	void Stop() noexcept;

	void Reset() noexcept;
	void Restart() noexcept;

	bool IsRunning() const noexcept;
	TimeSpan GetElapsed() const noexcept;

	static PerformanceTimer StartNew() noexcept;

public:
	PerformanceTimer& operator =(const PerformanceTimer& rhs) noexcept;

private:
	std::optional<DateTime> _startTime;
	TimeSpan _elapsed;
};