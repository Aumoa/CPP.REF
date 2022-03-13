// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAssert.h"
#include <chrono>

template<class T, class Clock = std::chrono::steady_clock>
class ScopedTimer
{
	using TimePoint = typename Clock::time_point;
	using Report = void(T::*)(float);

private:
	T* Instance;
	TimePoint StartTime;
	Report Reporter;

public:
	inline ScopedTimer(T* Instance, Report InReporter)
		: Instance(Instance)
		, StartTime(Clock::now())
		, Reporter(InReporter)
	{
		checkf(Instance, L"Instance is nullptr.");
	}

	inline ~ScopedTimer()
	{
		InternalStop();
	}

	inline void Stop()
	{
		checkf(Instance, L"Timer already stopped.");
		InternalStop();
	}

private:
	inline void InternalStop()
	{
		if (Instance)
		{
			TimePoint EndTime = Clock::now();
			auto Dur = EndTime - StartTime;
			(Instance->*Reporter)(std::chrono::duration_cast<std::chrono::duration<float>>(Dur).count());

			Instance = nullptr;
		}
	}
};