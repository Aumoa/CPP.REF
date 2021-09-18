// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SlateCommon.hlsli"

#define RENDER_MODE_IMAGE_SOURCE	0
#define RENDER_MODE_GLYPH			1

Texture2D<float4> gImageSource : register(t1);
SamplerState gLinearSampler : register(s0);
SamplerState gPointSampler : register(s1);
cbuffer _32BitConstants : register(b1)
{
	int gRenderMode;
};

Pixel Main(in Fragment frag)
{
	if (gRenderMode == RENDER_MODE_IMAGE_SOURCE)
	{
		Pixel px;
		px.Color = gImageSource.Sample(gLinearSampler, frag.TexCoord);
		return px;
	}
	else
	{
		Pixel px;
		px.Color = float4((float3)1, gImageSource.Sample(gPointSampler, frag.TexCoord).x);
		return px;
	}
}