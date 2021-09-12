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
StructuredBuffer<SlateElement> gElements : register(t0);

Fragment Main(in uint vId : SV_VERTEXID, in uint iId : SV_INSTANCEID)
{
	SlateElement myElement = gElements[iId];

	Fragment frag;
	frag.Position = float4(GetSlateNDCLocation(gConstants, myElement, gPos[vId]), 1.0f);
	frag.TexCoord = gTex[vId];
	return frag;
}