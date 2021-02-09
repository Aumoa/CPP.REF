// Copyright 2020 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"
#include "../PrimitivePacking.hlsli"

uint gMaterialIndex : register(b0);

GeometryPixel PS_Main(in Fragment inFrag)
{
	float3 normal = normalize(inFrag.Normal);

	GeometryPixel oPx;
	oPx.Color = float4((float3)1.0f, 0);
	oPx.Normal = uint4(ConvSnorm16ToUInt16(normal), gMaterialIndex);
	return oPx;
}