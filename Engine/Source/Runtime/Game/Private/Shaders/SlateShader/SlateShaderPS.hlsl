// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SlateCommon.hlsli"

Texture2D<float4> gImageSource : register(t1);
SamplerState gSampler : register(s0);

Pixel Main(in Fragment frag)
{
	Pixel px;
	px.Color = gImageSource.Sample(gSampler, frag.TexCoord);
	return px;
}