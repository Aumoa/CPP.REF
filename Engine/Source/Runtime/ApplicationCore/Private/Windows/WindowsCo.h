// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__
#include "Platform/PlatformCommon.h"

namespace Ayla
{
	struct NWindowsCoStatics : public StaticClass
	{
		static void TryCoInitialize();
		static ComPtr<IWICImagingFactory2> GetImagingFactory();
	};
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif