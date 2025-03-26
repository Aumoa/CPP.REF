// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include "Platform/Generic/GenericPlatformTypes.h"

namespace Ayla
{
	struct WindowsPlatformTypes : public GenericPlatformTypes
	{
	};

	using PlatformTypes = WindowsPlatformTypes;
}

#endif