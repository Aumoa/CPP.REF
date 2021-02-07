// Copyright 2020 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"
#include "../GenericLighting.hlsli"

Texture2D<float4> gColorBuffer : register(t0);

SamplerState gSampler : register(s0);

HDRPixel PS_Main(in QuadFrag inFrag)
{
	float4 color = gColorBuffer.Sample(gSampler, inFrag.Tex);
	HDRPixel oPixel;
	oPixel.Color = color;
	return oPixel;
}