// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_OSX

#include "Platform/Unix/UnixPlatformAtomics.h"
#include "IntegralTypes.h"

namespace Ayla
{
    struct CORE_API OSXPlatformAtomics : public UnixPlatformAtomics
    {
    };

    using PlatformAtomics = OSXPlatformAtomics;
}

#endif
