// Copyright 2020 Aumoa.lib. All right reserved.

struct Vertex
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD;
	float3 Normal : NORMAL;
};

struct Fragment
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
};

struct ShaderCameraConstant
{
	row_major matrix World;
	row_major matrix ViewProj;
	row_major matrix WVP;
};

ConstantBuffer<ShaderCameraConstant> gCameraConstant : register(b0);

Fragment VS_Main(in Vertex inVertex)
{
	Fragment oFrag;
	oFrag.PosH = mul(float4(inVertex.Pos, 1.0f), gCameraConstant.WVP);
	oFrag.Color = float4(inVertex.Normal, 1.0f);
	oFrag.Normal = mul(inVertex.Normal, (float3x3)gCameraConstant.World);
	return oFrag;
}