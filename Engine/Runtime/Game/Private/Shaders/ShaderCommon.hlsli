// Copyright 2020 Aumoa.lib. All right reserved.

#ifndef __SHADERCOMMON_HLSLI__
#define __SHADERCOMMON_HLSLI__

struct Vertex
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
};

struct Fragment
{
	float4 PosH : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
};

struct GeometryPixel
{
	float4 Color : SV_TARGET0;
	uint4 Normal : SV_TARGET1;
};

struct ShaderObjectConstant
{
	row_major matrix World;
	row_major matrix WVP;
};

struct ShaderCameraConstant
{
	row_major matrix ViewProj;
	row_major matrix ViewProjInv;
	float3 Pos;
};

struct QuadFrag
{
	float4 PosH : SV_POSITION;
	float2 Tex : TEXCOORD;
};

struct HDRPixel
{
	float4 Color : SV_TARGET0;
};

struct LDRPixel
{
	float4 Color : SV_TARGET0;
};

struct Material
{
	float Ambient;
	float Diffuse;
	float Specular;
	float SpecExp;
};

struct Light
{
	uint Type;
	float3 Color;
	float Ambient;
	float Diffuse;
	float Specular;
	uint ShadowCast;
	row_major matrix ViewProj;
	float4 Ambiguous_01;
};

static float4 QuadPos[4] =
{
	float4(-1.0f, +1.0f, +0.0f, 1.0f),
	float4(+1.0f, +1.0f, +0.0f, 1.0f),
	float4(-1.0f, -1.0f, +0.0f, 1.0f),
	float4(+1.0f, -1.0f, +0.0f, 1.0f),
};

static float2 QuadTex[4] =
{
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f),
	float2(0.0f, 1.0f),
	float2(1.0f, 1.0f),
};

#endif