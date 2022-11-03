// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "CoreMinimal.h"

#undef interface
#define NOMINMAX

#pragma push_macro("TEXT")
#undef TEXT

#pragma pack(push, 8)

#include <Windows.h>
#include <WinSock2.h>
#include <MSWSock.h>

#pragma pack(pop)

#pragma pop_macro("TEXT")

#undef GetCommandLine
#undef Yield
#undef GetMessage
#undef GetObject

inline String FormatLastError(DWORD errorCode = GetLastError())
{
	static thread_local WCHAR buf[1024] = {};
	DWORD len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, 0, buf, 1024, NULL);
	if (len == 0)
	{
		return TEXT("Unknown error.");
	}

	return String(buf, len);
}

inline String FormatWSAError(int errorCode = WSAGetLastError())
{
	static thread_local WCHAR buf[1024] = {};
	DWORD len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, (DWORD)errorCode, 0, buf, 1024, NULL);
	if (len == 0)
	{
		return TEXT("Unknown error.");
	}

	return String(buf, len);
}

#endif