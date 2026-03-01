// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_LINUX

#include "Platform/Unix/UnixPlatformAtomics.h"

namespace Ayla
{
    struct CORE_API LinuxPlatformAtomics : public UnixPlatformAtomics
    {
    };

    using PlatformAtomics = LinuxPlatformAtomics;
}

#endif