// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#pragma pack(push, 4)
struct alignas(256) RHILight
{
	enum
	{
		LIGHT_TYPE_DIRECTIONAL,
	};

	uint32 Type;
	Vector3 LightColor;
	float Ambient;
	float Diffuse;
	float Specular;
	uint32 ShadowCast;
	Matrix4x4 ViewProj;
	Vector4 Ambiguous_01;
};
#pragma pack(pop)