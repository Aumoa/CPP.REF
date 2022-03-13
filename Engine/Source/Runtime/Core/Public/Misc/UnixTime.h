// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DateTime.h"

class UnixTime
{
	using clock = std::chrono::utc_clock;
	using time_point = clock::time_point;

private:
	std::optional<time_point> _tp;

public:
	constexpr UnixTime()
	{
	}

	constexpr UnixTime(const time_point& tp)
		: _tp(tp)
	{
	}

	constexpr bool IsValid() const
	{
		return _tp.has_value();
	}

	template<class TDateFormatter = libty::DateTimeFormat::Json>
	std::wstring ToString() const
	{
		Xassert(IsValid(), "Time is not setted.");
		return TDateFormatter::ToString(*_tp);
	}

	DateTime ToLocalTime() const
	{
		auto system_tp = clock::to_sys(GetTimePoint());
		return DateTime(system_tp);
	}

	time_point GetTimePoint() const
	{
		Xassert(IsValid(), "Time is not setted.");
		return *_tp;
	}

	explicit operator DateTime() const
	{
		return ToLocalTime();
	}

	static UnixTime FromSystemTime(const DateTime& dt)
	{
		auto system_tp = dt.GetTimePoint();
		return UnixTime(clock::from_sys(system_tp));
	}

	static UnixTime Now()
	{
		return UnixTime(clock::now());
	}

	template<class TDateFormatter = libty::DateTimeFormat::Json>
	static bool TryParse(std::wstring_view time, UnixTime& outDateTime)
	{
		time_point tp;
		if (!TDateFormatter::TryParse(time, tp))
		{
			outDateTime = UnixTime();
			return false;
		}

		outDateTime = UnixTime(tp);
		return true;
	}

private:
	static void Xassert(bool x, std::string_view message, const std::source_location& source = std::source_location::current())
	{
		if (!x)
		{
			throw invalid_operation(message, source);
		}
	}
};