// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Platform/Linux/LinuxPlatformMisc.h"

#if PLATFORM_LINUX

#include <cerrno>

int32 LinuxPlatformMisc::GetLastError() noexcept
{
	return errno;
}

String LinuxPlatformMisc::FormatSystemCode(int32 ErrorCode) noexcept
{
	String Str = String::FromLiteral(strerror(errno));
	return Str;
}

#endif