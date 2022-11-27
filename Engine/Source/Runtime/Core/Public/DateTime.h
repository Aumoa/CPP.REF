// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DateTimeFormat.h"
#include "TimeSpan.h"
#include <chrono>
#include <optional>

class DateTime
{
public:
	using clock = std::chrono::system_clock;
	using time_point = clock::time_point;

private:
	time_point _tp;

public:
	inline constexpr DateTime()
	{
	}

	inline constexpr DateTime(const time_point& tp)
		: _tp(tp)
	{
	}

	template<class TDateFormatter = DateTimeFormat::Json>
	inline String ToString() const noexcept
	{
		return TDateFormatter::ToString(std::chrono::zoned_time(std::chrono::current_zone(), _tp));
	}

	inline String ToString(String format) const
	{
		format = String::Format(TEXT("{{:{}}}"), format);
		return String::Format(format, GetTimePoint());
	}

	inline constexpr const time_point& GetTimePoint() const noexcept
	{
		return _tp;
	}

	template<class T>
	constexpr DateTime GetFloor() const noexcept
	{
		return DateTime(std::chrono::floor<T>(GetTimePoint()));
	}

	static DateTime Now() noexcept
	{
		return DateTime(clock::now());
	}

	template<class TDateFormatter = DateTimeFormat::Json>
	static bool TryParse(std::wstring_view time, DateTime& outDateTime) noexcept
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

	inline constexpr auto GetYears() const noexcept { return std::chrono::floor<std::chrono::years>(_tp); }
	inline constexpr auto GetMonths() const noexcept { return std::chrono::floor<std::chrono::months>(_tp); }
	inline constexpr auto GetDays() const noexcept { return std::chrono::floor<std::chrono::days>(_tp); }
	inline constexpr auto GetHours() const noexcept { return std::chrono::floor<std::chrono::hours>(_tp); }
	inline constexpr auto GetMinutes() const noexcept { return std::chrono::floor<std::chrono::minutes>(_tp); }
	inline constexpr auto GetSeconds() const noexcept { return std::chrono::floor<std::chrono::seconds>(_tp); }
	inline constexpr auto GetMilliseconds() const noexcept { return std::chrono::floor<std::chrono::milliseconds>(_tp); }
	inline constexpr auto GetMicroseconds() const noexcept { return std::chrono::floor<std::chrono::microseconds>(_tp); }
	inline constexpr auto GetNanoseconds() const noexcept { return std::chrono::floor<std::chrono::nanoseconds>(_tp); }

	inline constexpr auto GetYMD() const noexcept { return std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(_tp)); }
	inline constexpr auto GetHMD() const noexcept { return std::chrono::hh_mm_ss(_tp - std::chrono::floor<std::chrono::days>(_tp)); }

	inline constexpr TimeSpan operator -(const DateTime& rhs) const noexcept { return TimeSpan(_tp - rhs._tp); }
	inline constexpr DateTime operator +(const TimeSpan& rhs) const noexcept { return DateTime(_tp + (std::chrono::microseconds)rhs); }
	inline constexpr DateTime operator -(const TimeSpan& rhs) const noexcept { return DateTime(_tp - (std::chrono::microseconds)rhs); }
};