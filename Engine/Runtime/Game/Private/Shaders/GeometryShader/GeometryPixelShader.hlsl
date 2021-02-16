// Copyright 2020 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"
#include "../PrimitivePacking.hlsli"

uint gMaterialIndex : register(b0);

Texture2D<float4> gDiffuseMap : register(t0);
Texture2D<float4> gNormalMap : register(t1);
SamplerState gSampler : register(s0);

GeometryPixel PS_Main(in Fragment inFrag)
{
	float3 VertexColor = 1.0f;

	float3 normal = normalize(inFrag.Normal);

	float4 sampleColor = gDiffuseMap.Sample(gSampler, inFrag.Tex);
	float3 color = lerp(VertexColor, sampleColor.xyz, sampleColor.w);

	GeometryPixel oPx;
	oPx.Color = float4(color, 1.0f);
	oPx.Normal = uint4(ConvSnorm16ToUInt16(normal), gMaterialIndex);
	return oPx;
}