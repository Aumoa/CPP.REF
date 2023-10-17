// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Color.h"

struct alignas(256) NSlateShaderRenderParams
{
	Color TintColor;
	float RenderOpacity;
	int32 RenderStates;

	static constexpr int32 RS_HasTexture = 0x01;
	static constexpr int32 RS_AlphaOnly = 0x02;
};