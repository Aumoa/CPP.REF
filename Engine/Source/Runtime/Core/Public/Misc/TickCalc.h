// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <chrono>
#include <numeric>
#include <list>

template<class TTimer = std::chrono::steady_clock>
class TickCalc
{
	std::optional<typename TTimer::time_point> TimePoint;
	std::chrono::duration<float> LastDeltaSeconds = std::chrono::duration<float>(0);

public:
	TickCalc()
	{
	}

	~TickCalc()
	{
	}

	std::chrono::duration<float> DoCalc()
	{
		using namespace std;

		typename TTimer::time_point Now = TTimer::now();
		std::chrono::duration<float> DeltaSeconds = 0ns;

		if (TimePoint.has_value())
		{
			DeltaSeconds = Now - *TimePoint;
		}
		TimePoint = Now;

		LastDeltaSeconds = DeltaSeconds;
		AddTime(DeltaSeconds.count());
		return LastDeltaSeconds;
	}

	std::chrono::duration<float> GetDeltaSeconds() const
	{
		return LastDeltaSeconds;
	}

	float GetAverageSeconds() const
	{
		size_t Samples = ElapsedSeconds.size();
		if (Samples)
		{
			float Avg = std::accumulate(ElapsedSeconds.begin(), ElapsedSeconds.end(), 0.0f);
			return Avg / Samples;
		}
		else
		{
			return 0;
		}
	}

	float GetAverageFPS() const
	{
		float Avg = GetAverageSeconds();
		if (Avg == 0)
		{
			return 0;
		}
		else
		{
			return 1.0f / Avg;
		}
	}

private:
	std::list<float> ElapsedSeconds;
	float ElapsedTime = 0;

	void AddTime(float InElapsedSeconds)
	{
		ElapsedSeconds.emplace_back(InElapsedSeconds);
		ElapsedTime += InElapsedSeconds;

		if (ElapsedTime > 1.0f)
		{
			ElapsedTime -= ElapsedSeconds.front();
			ElapsedSeconds.erase(ElapsedSeconds.begin());
		}
	}
};