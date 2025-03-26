// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Diagnostics/PerformanceTimer.h"

namespace Ayla
{
	PerformanceTimer::PerformanceTimer(const PerformanceTimer& Rhs) noexcept
		: StartTime(Rhs.StartTime)
		, Elapsed(Rhs.Elapsed)
	{
	}

	PerformanceTimer::PerformanceTimer(PerformanceTimer&& Rhs) noexcept
		: StartTime(std::move(Rhs.StartTime))
		, Elapsed(Rhs.Elapsed)
	{
		Rhs.Elapsed = std::chrono::seconds(0);
	}

	void PerformanceTimer::Start() noexcept
	{
		StartTime = DateTime::Now();
	}

	void PerformanceTimer::Stop() noexcept
	{
		if (!IsRunning())
		{
			return;
		}

		TimeSpan Dur = DateTime::Now() - StartTime.value();
		Elapsed += Dur;
		StartTime.reset();
	}

	void PerformanceTimer::Reset() noexcept
	{
		Elapsed = TimeSpan::FromSeconds(0);
		StartTime.reset();
	}

	void PerformanceTimer::Restart() noexcept
	{
		Elapsed = TimeSpan::FromSeconds(0);
		StartTime = DateTime::Now();
	}

	bool PerformanceTimer::IsRunning() const noexcept
	{
		return StartTime.has_value();
	}

	TimeSpan PerformanceTimer::GetElapsed() const noexcept
	{
		if (IsRunning())
		{
			auto Dur = DateTime::Now() - StartTime.value();
			return Elapsed + Dur;
		}
		else
		{
			return Elapsed;
		}
	}

	PerformanceTimer PerformanceTimer::StartNew() noexcept
	{
		return PerformanceTimer(std::in_place);
	}

	PerformanceTimer& PerformanceTimer::operator =(const PerformanceTimer& Rhs) noexcept
	{
		StartTime = Rhs.StartTime;
		Elapsed = Rhs.Elapsed;
		return *this;
	}

	PerformanceTimer& PerformanceTimer::operator =(PerformanceTimer&& Rhs) noexcept
	{
		StartTime = std::move(Rhs.StartTime);
		Elapsed = Rhs.Elapsed;
		Rhs.Elapsed = std::chrono::seconds(0);
		return *this;
	}
}