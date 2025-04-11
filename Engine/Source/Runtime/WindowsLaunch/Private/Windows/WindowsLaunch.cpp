// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Windows/WindowsLaunch.h"
#include "Platform/PlatformCommon.h"

namespace Ayla
{
	WindowsLaunch::WindowsLaunch()
	{
		ApplicationPointer = GetModuleHandleW(nullptr);
	}

	void* WindowsLaunch::GetApplicationPointer()
	{
		return ApplicationPointer;
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif