// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_OSX

#include "Platform/Generic/GenericPlatformTypes.h"

namespace Ayla
{
	struct OSXPlatformTypes : public GenericPlatformTypes
	{
	};

	using PlatformTypes = OSXPlatformTypes;
}

#endif
