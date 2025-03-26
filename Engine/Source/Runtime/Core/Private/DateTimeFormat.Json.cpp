// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "DateTimeFormat.Json.h"
#include "DateTime.h"
#include "Platform/PlatformLocalization.h"
#include "DateTimeFormat.LocalTimeOffset.h"
#include <chrono>

namespace Ayla::DateTimeFormat
{
	String Json::ToString(const DateTime& Time)
	{
		const auto [YMD, HMS] = Time.GetYMDHMS();

		if (Time.IsUTC())
		{
			return String::Format(TEXT("{:0>2}-{:0>2}-{:0>2}T{:0>2}:{:0>2}:{:0>2}.{:0>3}Z"),
				(int)YMD.year(), (unsigned int)YMD.month(), (unsigned int)YMD.day(),
				HMS.hours().count(), HMS.minutes().count(), HMS.seconds().count(),
				HMS.subseconds().count());
		}
		else
		{
			const LocalTimeOffset& Off = LocalTimeOffset::Get(Time);
			return String::Format(TEXT("{:0>2}-{:0>2}-{:0>2}T{:0>2}:{:0>2}:{:0>2}.{:0>3}{}{:0>2}:{:0>2}"),
				(int)YMD.year(), (unsigned int)YMD.month(), (unsigned int)YMD.day(),
				HMS.hours().count(), HMS.minutes().count(), HMS.seconds().count(),
				HMS.subseconds().count(),
				Off.Op,
				Off.Hours.count(), Off.Minutes.count());
		}
	}
}