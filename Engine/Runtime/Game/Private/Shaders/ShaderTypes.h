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
}

#define row_major

#endif

struct Payload
{
	float4 Color;
	float3 Pos;
};

struct RayFragment
{
	float3 PosW;
	float2 Tex;
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

#if defined(__cplusplus)

#undef row_major

#endif