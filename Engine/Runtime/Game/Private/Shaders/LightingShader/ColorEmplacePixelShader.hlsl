// Copyright 2020 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"
#include "../GenericLighting.hlsli"
#include "../PrimitivePacking.hlsli"

Texture2D<float4> gColorBuffer : register(t0);
Texture2D<uint4> gNormalBuffer : register(t1);

SamplerState gSampler : register(s0);

HDRPixel PS_Main(in QuadFrag inFrag)
{
	float4 color = gColorBuffer.Sample(gSampler, inFrag.Tex);
	uint4 normal = gNormalBuffer[(uint2)inFrag.PosH.xy];

	HDRPixel oPixel;
	oPixel.Color = color;
	if (normal.w == 9)
	{
		oPixel.Color = 0.5f;
	}

	return oPixel;
}