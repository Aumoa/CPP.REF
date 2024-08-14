// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_UNIX

#include "Platform/Generic/GenericPlatformTypes.h"

struct UnixPlatformTypes : public GenericPlatformTypes
{
};

#endif