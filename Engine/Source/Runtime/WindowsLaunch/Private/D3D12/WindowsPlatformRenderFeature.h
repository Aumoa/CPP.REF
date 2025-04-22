// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformRenderFeature.h"
#include "WindowsPlatformRenderFeature.gen.h"

namespace Ayla
{
	ACLASS()
	class WINDOWSLAUNCH_API WindowsPlatformRenderFeature : public PlatformRenderFeature
	{
		GENERATED_BODY()

	public:
		virtual RPtr<Graphics> CreateGraphics(RenderAPI api) override;
	};
}