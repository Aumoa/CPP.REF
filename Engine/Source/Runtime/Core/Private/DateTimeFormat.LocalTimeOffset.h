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

	private:
		// Fallback implementation for compilers without std::chrono::current_zone support
		static LocalTimeOffset CalculateFallbackOffset(const DateTime& ReferenceTime);

	public:
		static const LocalTimeOffset& Get(const DateTime& ReferenceTime)
		{
			static const LocalTimeOffset Info = [&ReferenceTime]()
			{
				// Use fallback implementation for older compilers
				return CalculateFallbackOffset(ReferenceTime);
			}();

			return Info;
		}
	};
}