// Copyright 2020 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"

ConstantBuffer<ShaderObjectConstant> gCameraConstant : register(b0);

Fragment VS_Main(in Vertex inVertex)
{
	Fragment oFrag;
	oFrag.PosH = mul(float4(inVertex.Pos, 1.0f), gCameraConstant.WVP);
	oFrag.Normal = mul(inVertex.Normal, (float3x3)gCameraConstant.World);
	return oFrag;
}