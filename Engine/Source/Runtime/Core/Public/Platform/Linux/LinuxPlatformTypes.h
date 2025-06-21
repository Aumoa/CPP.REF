// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_LINUX

#include "Platform/Generic/GenericPlatformTypes.h"

namespace Ayla
{
	struct LinuxPlatformMacros : public GenericPlatformTypes
	{
	};

	using PlatformTypes = LinuxPlatformMacros;
}

#endif