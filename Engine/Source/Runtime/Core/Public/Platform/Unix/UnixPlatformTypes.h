// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_LINUX

#include "Platform/Generic/GenericPlatformTypes.h"
#include <sys/types.h>

namespace Ayla
{
	struct UnixPlatformTypes : public GenericPlatformTypes
	{
		using ssize_t = ::ssize_t;
	};
}

#endif