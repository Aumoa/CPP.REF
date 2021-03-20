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
#define LightType_Point 2
#define DirectionalLight_Direction Ambiguous_01.xyz
#define PointLight_Position Ambiguous_01.xyz
#define PointLight_Constant Ambiguous_02.x
#define PointLight_Linear Ambiguous_02.y
#define PointLight_Quad Ambiguous_02.z

struct GeneralLight
{
	uint Type;
	float Ambient;
	float Diffuse;
	float Specular;
	float3 Color;
	float pad0;
	float4 Ambiguous_01;
	float4 Ambiguous_02;
};

#define Payload_MaxRecursive 3
#define PayloadType_OpaqueHit 0
#define PayloadType_ShadowCast 1

struct Payload
{
	uint Attributes;
	float3 Pos;
	float4 Color;
	float3 Normal;
};

#if defined(__cplusplus)

}

#undef row_major

#undef LightType_Directional
#undef DirectionalLight_Direction
#undef PointLight_Position
#undef PointLight_Constant
#undef PointLight_Linear
#undef PointLight_Quad

#undef PayloadType_OpaqueHit
#undef PayloadType_ShadowCast

#pragma pack(pop)

#endif