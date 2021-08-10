// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SlateCommon.hlsli"

Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

Pixel Main(in Fragment frag)
{
	Pixel px;
	px.Color = gTexture.Sample(gSampler, frag.TexCoord);
	return px;
}