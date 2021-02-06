// Copyright 2020 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"

Texture2D<float4> gColorBuffer : register(t0);
Texture2D<float4> gNormalBuffer : register(t1);
Texture2D<float> gDepthBuffer : register(t2);

SamplerState gSampler : register(s0);

HDRPixel PS_Main(in QuadFrag inFrag)
{
	HDRPixel oPixel;
	oPixel.Color = gColorBuffer.Sample(gSampler, inFrag.Tex);
	return oPixel;
}