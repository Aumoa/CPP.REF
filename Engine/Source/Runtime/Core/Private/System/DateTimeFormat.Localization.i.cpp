// Copyright 2020-2023 Aumoa.lib. All right reserved.

import Core;

#include "System/LanguageSupportMacros.h"

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