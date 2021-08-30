// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <chrono>

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
		return _lastDeltaSeconds;
	}

	std::chrono::duration<float> GetDeltaSeconds() const
	{
		return _lastDeltaSeconds;
	}
};