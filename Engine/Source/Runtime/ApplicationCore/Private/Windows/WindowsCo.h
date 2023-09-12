// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS

#include "WindowsCommon.h"

struct NWindowsCoStatics : public StaticClass
{
	static void TryCoInitialize();
	static ComPtr<IWICImagingFactory2> GetImagingFactory();
};

#endif