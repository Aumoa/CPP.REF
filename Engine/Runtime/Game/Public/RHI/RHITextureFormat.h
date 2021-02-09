// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class ERHITextureFormat
{
	Unknown,
	R16G16B16A16_FLOAT						= 10,
	R16G16B16A16_UNORM						= 11,
	R16G16B16A16_UINT 						= 12,
	R16G16B16A16_SNORM						= 13,
	R16G16B16A16_SINT 						= 14,
	R8G8B8A8_UNORM							= 28,
	R24G8_TYPELESS							= 44,
	D24_UNORM_S8_UINT						= 45,
	R24_UNORM_X8_TYPELESS					= 46,
	X24_UNORM_G8_UINT						= 47,
	B8G8R8A8_UNORM							= 87,
};

GAME_API bool IsUnknownFormat(ERHITextureFormat inFormat);
GAME_API bool IsColorFormat(ERHITextureFormat inFormat);
GAME_API bool IsDepthStencilFormat(ERHITextureFormat inFormat);