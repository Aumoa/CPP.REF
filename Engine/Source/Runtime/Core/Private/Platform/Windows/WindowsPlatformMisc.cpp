// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Platform/Windows/WindowsPlatformMisc.h"

#if PLATFORM_WINDOWS

#include "Platform/PlatformLocalization.h"
#include "WindowsCore.h"

int32 PlatformMisc::GetLastError() noexcept
{
	return (int32)::GetLastError();
}

String PlatformMisc::FormatSystemCode(int32 InSystemCode) noexcept
{
	static thread_local WCHAR Buf[1024] = {};
	DWORD Len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, (DWORD)InSystemCode, 0, Buf, 1024, NULL);
	if (Len == 0)
	{
		return TEXT("Unknown error.");
	}

	return String(Buf, Len);
}

#endif