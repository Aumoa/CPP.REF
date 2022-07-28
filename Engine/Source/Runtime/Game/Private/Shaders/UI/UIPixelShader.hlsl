// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Shaders/UI/ShaderUICommon.hlsli"

struct Pixel
{
	float4 Color : SV_TARGET0;
};

Pixel PSMain(in Fragment frag)
{
	Pixel pixel;
	pixel.Color = frag.TintColor;
	return pixel;
}