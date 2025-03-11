// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include "Platform/Generic/GenericPlatformMisc.h"
#include "System/String.h"

struct CORE_API WindowsPlatformMisc : public GenericPlatformMisc
{
	static int32 GetLastError() noexcept;
	static String FormatSystemCode(int32 ErrorCode) noexcept;
};

using PlatformMisc = WindowsPlatformMisc;

#endif