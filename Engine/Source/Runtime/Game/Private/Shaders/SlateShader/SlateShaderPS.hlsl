// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SlateCommon.hlsli"

Texture2D<float4> gImageSource : register(t0);
SamplerState gLinearSampler : register(s0);

Pixel Main(in Fragment frag)
{
	Pixel px;
	px.Color = gImageSource.Sample(gLinearSampler, frag.TexCoord);
	return px;
}