// Copyright 2020-2024 Aumoa.lib. All right reserved.

#define __ALLOW_PLATFORM_COMMON_H__
#include "System/LanguageSupportMacros.h"
#include "Platform/PlatformCommon.h"

import Core;

#if PLATFORM_WINDOWS

int32 WindowsPlatformMisc::GetLastError() noexcept
{
	return (int32)::GetLastError();
}

String WindowsPlatformMisc::FormatSystemCode(int32 InSystemCode) noexcept
{
	static thread_local WCHAR Buf[1024] = {};
	DWORD Len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, (DWORD)InSystemCode, 0, Buf, 1024, NULL);
	if (Len == 0)
	{
		return TEXT("Unknown error.");
	}

	return String(Buf, Len);
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif