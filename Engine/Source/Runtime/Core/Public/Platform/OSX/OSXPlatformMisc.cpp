// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/OSX/OSXPlatformMisc.h"

#if PLATFORM_OSX

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/PlatformLocalization.h"
#include "Platform/PlatformCommon.h"

namespace Ayla
{
	int32 PlatformMisc::GetLastError() noexcept
	{
        return (int32)(errno);
	}

	String PlatformMisc::FormatSystemCode(int32 InSystemCode) noexcept
	{
		static thread_local char Buf[1024] = {};
        if (strerror_r(InSystemCode, Buf, sizeof(Buf)) == 0)
        {
            return String::FromLiteral(Buf);
        }
        else
        {
			return TEXT("Unknown error.");
        }
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif
