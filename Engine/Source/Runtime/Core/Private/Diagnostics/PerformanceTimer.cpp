// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Diagnostics/PerformanceTimer.h"

PerformanceTimer::PerformanceTimer() noexcept
{
}

PerformanceTimer::PerformanceTimer(const PerformanceTimer& rhs) noexcept
	: _startTime(rhs._startTime)
	, _elapsed(rhs._elapsed)
{
}

PerformanceTimer::~PerformanceTimer()
{
}

void PerformanceTimer::Start() noexcept
{
	if (!_startTime.has_value())
	{
		_startTime = DateTime::Now();
	}
}

void PerformanceTimer::Stop() noexcept
{
	if (_startTime.has_value())
	{
		TimeSpan elapsed = DateTime::Now() - *_startTime;
		_elapsed += elapsed;
		_startTime.reset();
	}
}

void PerformanceTimer::Reset() noexcept
{
	_startTime.reset();
	_elapsed = 0s;
}

void PerformanceTimer::Restart() noexcept
{
	_startTime = DateTime::Now();
	_elapsed = 0s;
}

bool PerformanceTimer::IsRunning() const noexcept
{
	return _startTime.has_value();
}

TimeSpan PerformanceTimer::GetElapsed() const noexcept
{
	if (_startTime.has_value())
	{
		return DateTime::Now() - *_startTime;
	}
	else
	{
		return _elapsed;
	}
}

PerformanceTimer PerformanceTimer::StartNew() noexcept
{
	PerformanceTimer timer;
	timer.Start();
	return timer;
}

PerformanceTimer& PerformanceTimer::operator =(const PerformanceTimer& rhs) noexcept
{
	_startTime = rhs._startTime;
	_elapsed = rhs._elapsed;
	return *this;
}