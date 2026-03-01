// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include "Platform/Generic/GenericPlatformTypes.h"

namespace Ayla
{
	struct WindowsPlatformTypes : public GenericPlatformTypes
	{
		using ssize_t = details::select_arch_t<sizeof(void*), int16, int32, int64>;
	};

	using PlatformTypes = WindowsPlatformTypes;
}

#endif