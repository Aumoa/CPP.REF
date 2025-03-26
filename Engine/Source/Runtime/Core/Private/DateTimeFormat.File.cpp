// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "DateTimeFormat.File.h"
#include "DateTime.h"
#include "Platform/PlatformLocalization.h"
#include <chrono>

namespace Ayla::DateTimeFormat
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