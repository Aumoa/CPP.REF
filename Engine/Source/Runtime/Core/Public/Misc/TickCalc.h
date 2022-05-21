// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <chrono>
#include <numeric>
#include <list>
#include <optional>

namespace libty::inline Core
{
	template<class Clock = std::chrono::steady_clock>
	class TickCalc
	{
		std::optional<typename Clock::time_point> TimePoint;
		std::chrono::duration<float> LastDeltaSeconds = std::chrono::duration<float>(0);

	public:
		TickCalc()
		{
			DoCalc();
		}

		~TickCalc()
		{
		}

		std::chrono::duration<float> DoCalc()
		{
			using namespace std;

			typename Clock::time_point Now = Clock::now();
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
			return AvgTime;
		}

		float GetAverageFPS() const
		{
			return AvgFPS;
		}

	private:
		uint64 Ticks = 0;
		float ElapsedTime = 0;

		float AvgTime = 0;
		float AvgFPS = 0;

		void AddTime(float InElapsedSeconds)
		{
			ElapsedTime += InElapsedSeconds;
			++Ticks;

			if (ElapsedTime > 1.0f)
			{
				AvgTime = ElapsedTime / Ticks;
				AvgFPS = Ticks * ElapsedTime;
				ElapsedTime = 0;
			}
		}
	};
}