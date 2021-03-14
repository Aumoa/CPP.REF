// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class ERHITextureFormat;

struct GAME_API RHITextureClearValue
{
	ERHITextureFormat Format;

	union
	{
		Color Color;
		struct
		{
			float Depth;
			uint8 Stencil;
		};
	};

	RHITextureClearValue();
	RHITextureClearValue(ERHITextureFormat inFormat);

	vs_property_get(bool, IsValid);
	vs_property_get(bool, IsColor);
	vs_property_get(bool, IsDepthStencil);
};