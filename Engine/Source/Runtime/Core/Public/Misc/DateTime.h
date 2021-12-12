// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <chrono>

template<class Clock = std::chrono::system_clock>
class DateTime
{
	Clock::time_point TimePoint;

public:
	constexpr DateTime(Clock::time_point TimePoint) : TimePoint(TimePoint)
	{
	}

	std::wstring ToString() const
	{
		using namespace std::chrono;

		auto Days = floor<days>(TimePoint);
		auto YMD = year_month_day(Days);

		auto TimeOfDay = TimePoint - Days;
		auto HMS = hh_mm_ss(TimeOfDay);
		return std::format(L"{} {}", YMD, HMS);
	}

	static constexpr DateTime Now()
	{
		return DateTime(Clock::now());
	}
};