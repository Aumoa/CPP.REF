// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_UNIX

#include "Platform/Generic/GenericPlatformProcess.h"

struct CORE_API UnixPlatformProcess : public GenericPlatformProcess
{
};

#endif