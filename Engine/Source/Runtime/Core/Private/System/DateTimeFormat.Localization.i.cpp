// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "System/DateTimeFormat.Localization.h"
#include "System/DateTime.h"
#include "Platform/PlatformLocalization.h"
#include <chrono>

namespace DateTimeFormat
{
	String Localization::ToString(const DateTime& Time)
	{
		if (Time.IsUTC())
		{
			return String::Format(TEXT("{}"), Time.GetTimePoint());
		}
		else
		{
			return String::Format(TEXT("{}"), Time.GetLocalTime());
		}
	}
}