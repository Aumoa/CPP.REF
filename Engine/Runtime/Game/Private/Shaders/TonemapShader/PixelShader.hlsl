// Copyright 2020 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"

Texture2D<float4> gHDRBuffer : register(t0);

SamplerState gSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

LDRPixel PS_Main(in QuadFrag inFrag)
{
	LDRPixel oPixel;
	oPixel.Color = gHDRBuffer.Sample(gSampler, inFrag.Tex);
	return oPixel;
}