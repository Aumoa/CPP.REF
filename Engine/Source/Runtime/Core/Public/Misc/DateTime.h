// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exceptions.h"
#include "DateTimeFormat.h"
#include <chrono>
#include <optional>
#include <version>

class DateTime
{
public:
	using clock = std::chrono::system_clock;
	using time_point = clock::time_point;

private:
#if __cpp_lib_chrono >= 201907L
	const std::chrono::time_zone* _zone = std::chrono::current_zone();
	std::optional<std::chrono::local_time<clock::duration>> _tp;
#else
	std::optional<time_point> _tp;
#endif

public:
	DateTime()
	{
	}

	DateTime(const time_point& tp)
	{
#if __cpp_lib_chrono >= 201907L
		_tp = _zone->to_local(tp);
#else
		_tp = tp;
#endif
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

	auto GetTimePoint() const
	{
		Xassert(IsValid(), "Time is not setted.");
		return *_tp;
	}

	static DateTime Now()
	{
		return DateTime(clock::now());
	}

	template<class TDateFormatter = libty::DateTimeFormat::Json>
	static bool TryParse(std::wstring_view time, DateTime& outDateTime)
	{
		time_point tp;
		if (!TDateFormatter::TryParse(time, tp))
		{
			outDateTime = DateTime();
			return false;
		}

		outDateTime = DateTime(tp);
		return true;
	}

	template<class Dur, class Ret = Dur>
	Ret Floor() const requires requires
	{
		{ std::chrono::floor<Dur>(std::declval<decltype(_tp->time_since_epoch())>()) };
	}
	{
		Ret ret = Ret(std::chrono::floor<Dur>(_tp->time_since_epoch()));
		return ret;
	}

	auto GetYears() const { return Floor<std::chrono::years>(); }
	auto GetMonths() const { return Floor<std::chrono::months>(); }
	auto GetDays() const { return Floor<std::chrono::days>(); }
	auto GetHours() const { return Floor<std::chrono::hours>(); }
	auto GetMinutes() const { return Floor<std::chrono::minutes>(); }
	auto GetSeconds() const { return Floor<std::chrono::seconds>(); }
	auto GetMilliseconds() const { return Floor<std::chrono::milliseconds>(); }
	auto GetMicroseconds() const { return Floor<std::chrono::microseconds>(); }
	auto GetNanoseconds() const { return Floor<std::chrono::nanoseconds>(); }

	auto GetYMD() const
	{
		auto ymd = std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(*_tp));
		return ymd;
	}

	auto GetHMD() const
	{
		auto days = std::chrono::floor<std::chrono::days>(*_tp);
		auto hms = std::chrono::hh_mm_ss(*_tp - days);
		return hms;
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