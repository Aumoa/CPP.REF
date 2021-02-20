// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "../ShaderCommon.hlsli"

ConstantBuffer<ShaderObjectConstant> gCameraConstant : register(b0);

Fragment VS_Main(in Vertex inVertex)
{
	Fragment oFrag;
	oFrag.PosH = mul(float4(inVertex.Pos, 1.0f), gCameraConstant.WVP);
	oFrag.Tex = inVertex.Tex;
	oFrag.Normal = mul(inVertex.Normal, (float3x3)gCameraConstant.World);
	return oFrag;
}