// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Windows/WindowsLaunch.h"
#include "Platform/PlatformCommon.h"
#include "D3D12/WindowsPlatformRenderFeature.h"

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

	RPtr<PlatformRenderFeature> WindowsLaunch::CreatePlatformRenderFeature()
	{
		return New<WindowsPlatformRenderFeature>();
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif