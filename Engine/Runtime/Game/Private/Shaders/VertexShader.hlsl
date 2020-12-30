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

Fragment VS_Main(in Vertex inVertex)
{
	Fragment oFrag;
	oFrag.PosH = float4(inVertex.Pos, 1.0f);
	oFrag.Color = inVertex.Color;
	return oFrag;
}