// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_LINUX

#include "Platform/Unix/UnixPlatformLocalization.h"

struct CORE_API LinuxPlatformLocalization : public UnixPlatformLocalization
{
};

using PlatformLocalization = LinuxPlatformLocalization;

#endif