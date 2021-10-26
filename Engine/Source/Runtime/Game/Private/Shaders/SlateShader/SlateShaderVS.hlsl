// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SlateCommon.hlsli"

static float2 gPos[] =
{
	float2(0, +1),
	float2(+1, +1),
	float2(0, 0),
	float2(+1, 0),
};

static float2 gTex[] =
{
	float2(0, 1),
	float2(1, 1),
	float2(0, 0),
	float2(1, 0)
};

ConstantBuffer<SlateConstants> gConstants : register(b0);
ConstantBuffer<SlateElement> gElements : register(b1);

Fragment Main(in uint vId : SV_VERTEXID, in uint iId : SV_INSTANCEID)
{
	return GetSlateFragment(gConstants, gElements, gPos[vId], gTex[vId]);
}