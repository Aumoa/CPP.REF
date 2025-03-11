// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_LINUX

#include "Platform/Unix/UnixPlatformTypes.h"

struct LinuxPlatformTypes : public UnixPlatformTypes
{
};

using PlatformTypes = LinuxPlatformTypes;

#endif