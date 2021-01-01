// Copyright 2020 Aumoa.lib. All right reserved.

struct Vertex
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
};

struct Fragment
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
};

static Vertex gVertex[3] =
{
	{ float3(0, 1, 0), float4(1, 0, 0, 1) },
	{ float3(1, -1, 0), float4(0, 1, 0, 1) },
	{ float3(-1, -1, 0), float4(0, 0, 1, 1) },
};

Fragment VS_Main(uint iVertexID : SV_VERTEXID)
{
	Fragment oFrag;
	oFrag.PosH = float4(gVertex[iVertexID].Pos, 1.0f);
	oFrag.Color = gVertex[iVertexID].Color;
	return oFrag;
}