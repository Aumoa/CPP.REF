// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "HAL/RenderAPI.h"
#include "PlatformRenderFeature.gen.h"

namespace Ayla
{
	class Graphics;

	ACLASS()
	class RENDERCORE_API PlatformRenderFeature : public Object
	{
		GENERATED_BODY()

	public:
		virtual RPtr<Graphics> CreateGraphics(RenderAPI api) = 0;
	};
}