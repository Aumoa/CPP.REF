// Copyright 2020-2021 Aumoa.lib. All right reserved.

#if defined(__cplusplus)

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

inline namespace
{
	using float4 = Vector4;
	using float3 = Vector3;
	using float2 = Vector2;
	using matrix = Matrix4x4;
	using uint = uint32;
}

namespace ShaderTypes
{

#pragma pack(push, 1)
#define row_major

#endif

struct Payload
{
	float4 Color;
	float3 Pos;
	float3 Normal;
};

struct RayFragment
{
	float3 PosW;
	float2 Tex;
	float3 NormalW;
};

struct BuiltInAttr
{
	float2 Value;
};

struct ShaderCameraConstant
{
	row_major matrix Proj;
	row_major matrix ViewProj;
	row_major matrix ViewProjInv;
	float3 Pos;
};

struct RaytracingInstanceTransform
{
	row_major matrix World;
	row_major matrix WorldInvTranspose;
};

struct Material
{
	float Ambient;
	float Diffuse;
	float Specular;
	float SpecExp;
};

#define LightType_Directional 1
#define DirectionalLight_Direction Ambiguous_01

struct GeneralLight
{
	uint Type;
	float Ambient;
	float Diffuse;
	float Specular;
	float3 Color;
	float pad0;
	float4 Ambiguous_01;
};

#if defined(__cplusplus)

}

#undef row_major
#pragma pack(pop)

#endif