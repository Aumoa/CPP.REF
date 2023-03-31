// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/DateTimeFormat.h"
#include "CoreTypes/DateTime.h"
#include <numeric>

namespace DateTimeFormat
{
	static_assert(std::same_as<DateTime::time_point, DateTimeFormatter::time_point>, "Mismatch time point type.");
	static_assert(std::same_as<DateTime::local_time, DateTimeFormatter::local_time>, "Mismatch local time type.");

	const DateTime::time_point& DateTimeFormatter::GetTimePoint(const DateTime& Time)
	{
		return Time.GetTimePoint();
	}

	const DateTime::local_time& DateTimeFormatter::GetLocalTime(const DateTime& Time)
	{
		return Time.GetLocalTime();
	}

	String Localization::ToString(const DateTime& Time)
	{
		if (Time.IsUTC())
		{
			return String::Format(TEXT("{:%Y:%m:%d %a %H:%M:%S} UTC"), std::chrono::floor<std::chrono::seconds>(GetTimePoint(Time)));
		}
		else
		{
			return String::Format(TEXT("{:%Y:%m:%d %a %H:%M:%S} {}"), std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::floor<std::chrono::seconds>(GetLocalTime(Time))), String::FromLiteral(std::chrono::current_zone()->name()));
		}
	}

	String Json::ToString(const DateTime& Time)
	{
		if (Time.IsUTC())
		{
			return String::Format(TEXT("{:%FT%TZ}"), std::chrono::floor<std::chrono::seconds>(GetTimePoint(Time)));
		}
		else
		{
			return String::Format(TEXT("{:%FT%T%z}"), std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::floor<std::chrono::seconds>(GetLocalTime(Time))));
		}
	}

	String File::ToString(const DateTime& Time)
	{
		if (Time.IsUTC())
		{
			return String::Format(TEXT("{:%Y.%m.%d-%H.%M.%S}"), std::chrono::floor<std::chrono::seconds>(GetTimePoint(Time)));
		}
		else
		{
			return String::Format(TEXT("{:%Y.%m.%d-%H.%M.%S}"), std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::floor<std::chrono::seconds>(GetLocalTime(Time))));
		}
	}
}