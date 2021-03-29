// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"

Texture2D<float4> gHDRBuffer : register(t0);

SamplerState gSampler : register(s0);

LDRPixel Main(in QuadFrag inFrag)
{
	LDRPixel oPixel;
	oPixel.Color = gHDRBuffer.Sample(gSampler, inFrag.Tex);
	return oPixel;
}