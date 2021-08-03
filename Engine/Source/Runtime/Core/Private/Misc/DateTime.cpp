// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Misc/DateTime.h"

using namespace std::chrono;

DateTime::DateTime()
{
}

DateTime::DateTime(const _Myclock::time_point& tp)
	: _localTime(zoned_time(current_zone(), tp).get_local_time())
{
	auto ld = floor<days>(_localTime);
	_ymd = year_month_day(ld);
	_hms = hh_mm_ss(ld - _localTime);
}

DateTime DateTime::Now()
{
	return DateTime(_Myclock::now());
}