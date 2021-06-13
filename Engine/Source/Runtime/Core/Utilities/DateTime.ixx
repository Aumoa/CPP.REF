// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:DateTime;

import std.core;

using namespace std::chrono;

/// <summary>
/// Represents an instant in time, typically expressed as a date and time of day.
/// </summary>
export struct DateTime
{
private:
	using _Myclock = system_clock;
	using _Mydur = _Myclock::duration;

	local_time<_Mydur> _localTime;
	year_month_day _ymd;
	hh_mm_ss<_Mydur> _hms;

public:
	DateTime();
	DateTime(const _Myclock::time_point& tp);

	inline local_time<_Mydur> GetLocalTime() const { return _localTime; }
	inline year GetYear() const { return _ymd.year(); }
	inline month GetMonth() const { return _ymd.month(); }
	inline day GetDay() const { return _ymd.day(); }
	inline hours GetHours() const { return _hms.hours(); }
	inline minutes GetMinutes() const { return _hms.minutes(); }
	inline seconds GetSeconds() const { return _hms.seconds(); }
	inline hh_mm_ss<_Mydur>::precision GetSubseconds() const { return _hms.subseconds(); }

	/// <summary>
	/// Gets a <see cref="DateTime"/> instance that is set to the current date and time on this computer, expressed as the local time.
	/// </summary>
	static DateTime Now();
};