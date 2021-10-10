// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <chrono>
#include <numeric>
#include <list>

template<class TTimer = std::chrono::steady_clock>
class TickCalc
{
	std::optional<typename TTimer::time_point> _point;
	std::chrono::duration<float> _lastDeltaSeconds = std::chrono::duration<float>(0);

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

		typename TTimer::time_point now = TTimer::now();
		std::chrono::duration<float> deltaSeconds = 0ns;

		if (_point.has_value())
		{
			deltaSeconds = now - *_point;
		}
		_point = now;

		_lastDeltaSeconds = deltaSeconds;
		AddTime(deltaSeconds.count());
		return _lastDeltaSeconds;
	}

	std::chrono::duration<float> GetDeltaSeconds() const
	{
		return _lastDeltaSeconds;
	}

	float GetAverageSeconds() const
	{
		size_t samples = _elapsedTimes.size();
		if (samples)
		{
			float avg = std::accumulate(_elapsedTimes.begin(), _elapsedTimes.end(), 0.0f);
			return avg / samples;
		}
		else
		{
			return 0;
		}
	}

	float GetAverageFPS() const
	{
		float avg = GetAverageSeconds();
		if (avg == 0)
		{
			return 0;
		}
		else
		{
			return 1.0f / avg;
		}
	}

private:
	std::list<float> _elapsedTimes;
	float _elapsedTime = 0;

	void AddTime(float elapsedSeconds)
	{
		_elapsedTimes.emplace_back(elapsedSeconds);
		_elapsedTime += elapsedSeconds;

		if (_elapsedTime > 1.0f)
		{
			_elapsedTime -= _elapsedTimes.front();
			_elapsedTimes.erase(_elapsedTimes.begin());
		}
	}
};