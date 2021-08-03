// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <chrono>

/// <summary>
/// Represents an instant in time, typically expressed as a date and time of day.
/// </summary>
struct CORE_API DateTime
{
private:
	using _Myclock = std::chrono::system_clock;
	using _Mydur = _Myclock::duration;

	std::chrono::local_time<_Mydur> _localTime;
	std::chrono::year_month_day _ymd;
	std::chrono::hh_mm_ss<_Mydur> _hms;

public:
	DateTime();
	DateTime(const _Myclock::time_point& tp);

	inline std::chrono::local_time<_Mydur> GetLocalTime() const { return _localTime; }
	inline std::chrono::year GetYear() const { return _ymd.year(); }
	inline std::chrono::month GetMonth() const { return _ymd.month(); }
	inline std::chrono::day GetDay() const { return _ymd.day(); }
	inline std::chrono::hours GetHours() const { return _hms.hours(); }
	inline std::chrono::minutes GetMinutes() const { return _hms.minutes(); }
	inline std::chrono::seconds GetSeconds() const { return _hms.seconds(); }
	inline std::chrono::hh_mm_ss<_Mydur>::precision GetSubseconds() const { return _hms.subseconds(); }

	/// <summary>
	/// Gets a <see cref="DateTime"/> instance that is set to the current date and time on this computer, expressed as the local time.
	/// </summary>
	static DateTime Now();
};