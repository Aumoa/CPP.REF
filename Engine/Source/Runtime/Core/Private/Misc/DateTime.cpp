// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Misc/DateTime.h"

DateTime::DateTime()
{
}

DateTime::DateTime(const _Myclock::time_point& tp)
	: _localTime(std::chrono::zoned_time(std::chrono::current_zone(), tp).get_local_time())
{
	using namespace std::chrono;

	auto ld = floor<days>(_localTime);
	_ymd = year_month_day(ld);
	_hms = hh_mm_ss(ld - _localTime);
}

DateTime DateTime::Now()
{
	return DateTime(_Myclock::now());
}