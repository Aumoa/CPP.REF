// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exceptions.h"
#include "DateTimeFormat.h"
#include <chrono>
#include <optional>

class DateTime
{
	using clock = std::chrono::system_clock;
	using time_point = clock::time_point;

private:
	const std::chrono::time_zone* _zone = std::chrono::current_zone();
	std::optional<time_point> _tp;

public:
	DateTime()
	{
	}

	DateTime(const time_point& tp)
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
		return TDateFormatter::ToString(std::chrono::zoned_time(*_tp));
	}

	time_point GetTimePoint() const
	{
		Xassert(IsValid(), "Time is not setted.");
		return *_tp;
	}

	auto GetZonedTime() const
	{
		Xassert(IsValid(), "Time is not setted.");
		return std::chrono::zoned_time(_zone, *_tp);
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

private:
	static void Xassert(bool x, std::string_view message, const std::source_location& source = std::source_location::current())
	{
		if (!x)
		{
			throw invalid_operation(message, source);
		}
	}
};