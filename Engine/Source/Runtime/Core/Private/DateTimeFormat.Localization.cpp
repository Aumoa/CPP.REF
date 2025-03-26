// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "DateTimeFormat.Localization.h"
#include "DateTime.h"
#include "Platform/PlatformLocalization.h"
#include <chrono>

namespace Ayla::DateTimeFormat
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