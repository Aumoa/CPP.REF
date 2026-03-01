// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "DateTimeFormat.LocalTimeOffset.h"
#include <ctime>

#define __ALLOW_PLATFORM_COMMON_H__

#if PLATFORM_WINDOWS
#include "Platform/PlatformCommon.h"
#endif

namespace Ayla::DateTimeFormat
{
	LocalTimeOffset LocalTimeOffset::CalculateFallbackOffset(const DateTime& ReferenceTime)
	{
		using namespace std::chrono_literals;

#if PLATFORM_WINDOWS
		// Windows implementation using GetTimeZoneInformation
		TIME_ZONE_INFORMATION tzi;
		DWORD result = GetTimeZoneInformation(&tzi);

		if (result == TIME_ZONE_ID_INVALID)
		{
			// Default to UTC if we can't get timezone info
			return { '+', 0h, 0min };
		}

		// Windows bias is in minutes and represents UTC = local time + bias
		// We want local = UTC + offset, so offset = -bias
		long biasMinutes = tzi.Bias;
		if (result == TIME_ZONE_ID_DAYLIGHT)
		{
			biasMinutes += tzi.DaylightBias;
		}
		else if (result == TIME_ZONE_ID_STANDARD)
		{
			biasMinutes += tzi.StandardBias;
		}

		const long offsetMinutes = -biasMinutes;
		const char_t op = offsetMinutes < 0 ? '-' : '+';
		const long absOffsetMinutes = offsetMinutes < 0 ? -offsetMinutes : offsetMinutes;

		return {
			op,
			std::chrono::hours{ absOffsetMinutes / 60 },
			std::chrono::minutes{ absOffsetMinutes % 60 }
		};

#else
		// POSIX implementation using localtime_r
		const auto timePoint = ReferenceTime.ToUTC().GetTimePoint();
		const time_t time = std::chrono::system_clock::to_time_t(timePoint);

		struct tm utcTm, localTm;

#if defined(__STDC_LIB_EXT1__) || defined(_MSC_VER)
		gmtime_s(&utcTm, &time);
		localtime_s(&localTm, &time);
#else
		gmtime_r(&time, &utcTm);
		localtime_r(&time, &localTm);
#endif

		// Calculate difference between local and UTC
		const time_t utcTime = mktime(&utcTm);
		const time_t localTime = mktime(&localTm);

		// Get timezone offset in seconds
#ifdef __USE_MISC
		const long offsetSeconds = localTm.tm_gmtoff;
#else
			// Fallback calculation
		const long offsetSeconds = static_cast<long>(difftime(localTime, utcTime));
#endif

		const char_t op = offsetSeconds < 0 ? '-' : '+';
		const long absOffsetSeconds = offsetSeconds < 0 ? -offsetSeconds : offsetSeconds;
		const long offsetMinutes = absOffsetSeconds / 60;

		return {
			op,
			std::chrono::hours{ offsetMinutes / 60 },
			std::chrono::minutes{ offsetMinutes % 60 }
		};
#endif
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__