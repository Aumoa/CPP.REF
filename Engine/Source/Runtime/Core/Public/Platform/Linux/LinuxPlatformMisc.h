// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_LINUX

#include "Platform/Generic/GenericPlatformMisc.h"
#include "System/IntegralTypes.h"
#include "System/String.h"

struct CORE_API LinuxPlatformMisc : public GenericPlatformMisc
{
	static int32 GetLastError() noexcept;
	static String FormatSystemCode(int32 ErrorCode) noexcept;
};

using PlatformMisc = LinuxPlatformMisc;

#endif