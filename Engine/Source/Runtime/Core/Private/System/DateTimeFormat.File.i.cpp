// Copyright 2020-2024 Aumoa.lib. All right reserved.

#include "System/LanguageSupportMacros.h"

import Core;

namespace DateTimeFormat
{
	String File::ToString(const DateTime& Time)
	{
		const auto YMD = Time.GetYMD();

		if (Time.IsUTC())
		{
			return String::Format(TEXT("{:0>2}-{:0>2}-{:0>2} UTC"),
				(int)YMD.year(), (unsigned int)YMD.month(), (unsigned int)YMD.day());
		}
		else
		{
			return String::Format(TEXT("{:0>2}-{:0>2}-{:0>2}"),
				(int)YMD.year(), (unsigned int)YMD.month(), (unsigned int)YMD.day());
		}
	}
}