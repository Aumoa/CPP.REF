// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Shaders/UI/ShaderUICommon.hlsli"

struct DrawInstance
{
	float3x2 Transform2D;
	float4 TintColor;
};

float4 gVertices[4] =
{
	float4(-1.0f, +1.0f, 0.0f, 1.0f),
	float4(+1.0f, +1.0f, 0.0f, 1.0f),
	float4(-1.0f, -1.0f, 0.0f, 1.0f),
	float4(+1.0f, -1.0f, 0.0f, 1.0f),
};

StructuredBuffer<DrawInstance> gInstances : register(t0, space1);

Fragment VSMain(in uint vertexId : SV_VERTEXID, in uint instanceId : SV_INSTANCEID)
{
	Fragment frag;
	frag.Position = gVertices[vertexId];
	frag.TintColor = gInstances[instanceId].TintColor;
	return frag;
}