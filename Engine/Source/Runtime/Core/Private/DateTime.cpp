// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "DateTime.h"
#include <ctime>

namespace Ayla
{
	int32 DateTime::s_UTCOffset;

	void DateTime::TryStaticInitialize()
	{
		static int s_Init = []() -> int
		{
			auto now = std::time(nullptr);
			std::tm local;
			std::tm gm;

#if PLATFORM_WINDOWS
			localtime_s(&local, &now);
			gmtime_s(&gm, &now);
#else
			localtime_r(&now, &local);
			gmtime_r(&now, &gm);
#endif
			s_UTCOffset = local.tm_hour - gm.tm_hour;
			return 0;
		}();
	}

	auto DateTime::ToLocal(const time_point& value) -> time_point
	{
		TryStaticInitialize();
		return value + std::chrono::hours(s_UTCOffset);
	}

	auto DateTime::ToSys(const local_time& value) -> time_point
	{
		TryStaticInitialize();
		return time_point(value.time_since_epoch() - std::chrono::hours(s_UTCOffset));
	}
}