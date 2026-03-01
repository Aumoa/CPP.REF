// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_OSX

#include "Platform/Unix/UnixPlatformTypes.h"

namespace Ayla
{
	struct OSXPlatformTypes : public UnixPlatformTypes
	{
	};

	using PlatformTypes = OSXPlatformTypes;
}

#endif
