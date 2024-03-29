// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include "Platform/Generic/GenericPlatformTypes.h"

struct WindowsPlatformTypes : public GenericPlatformTypes
{
};

using PlatformTypes = WindowsPlatformTypes;

#endif