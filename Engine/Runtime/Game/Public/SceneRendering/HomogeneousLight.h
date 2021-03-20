// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#pragma pack(push, 1)

struct HomogeneousLight
{
	enum Lights : uint32
	{
		INDEX_NONE,
		INDEX_DIRECTIONAL,
		INDEX_POINT
	};

	Lights LightIndex;
	float Ambient;
	float Diffuse;
	float Specular;
	Vector3 Color;
	float pad;

	union
	{
		struct
		{
			Vector4 Pad0;
			Vector4 Pad1;
		} Pads;
		
		struct
		{
			Vector3 Direction;
			float pad;
		}
		DirectionalLightAtt;

		struct
		{
			Vector3 Position;
			float pad;
			float Constant;
			float Linear;
			float Quad;
		}
		PointLightAtt;
	};

	HomogeneousLight()
		: LightIndex(INDEX_NONE)
		, Ambient(0)
		, Diffuse(0)
		, Specular(0)
		, Color(Vector3::Zero)
	{
	}

	HomogeneousLight(const HomogeneousLight& rhs)
	{
		memcpy(this, &rhs, sizeof(HomogeneousLight));
	}
};

#pragma pack(pop)