// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "DateTime.h"
#include <chrono>

namespace Ayla::DateTimeFormat
{
	struct LocalTimeOffset
	{
		const char_t Op;
		const std::chrono::hours Hours;
		const std::chrono::minutes Minutes;

		static const LocalTimeOffset& Get(const DateTime& ReferenceTime)
		{
			static const LocalTimeOffset Info = [&ReferenceTime]()
			{
				using namespace std::chrono_literals;

				const auto* Zone = std::chrono::current_zone();
				const std::chrono::sys_info SysInfo = Zone->get_info(ReferenceTime.ToUTC().GetTimePoint());

				const char_t Op = SysInfo.offset < 0s ? '-' : '+';
				const auto Absolute = Op == '-' ? -SysInfo.offset : SysInfo.offset;
				const auto Hours = std::chrono::floor<std::chrono::hours>(Absolute);
				return LocalTimeOffset
				{
					.Op = Op,
					.Hours = Hours,
					.Minutes = std::chrono::floor<std::chrono::minutes>(Absolute) - Hours
				};
			}();

			return Info;
		}
	};
}