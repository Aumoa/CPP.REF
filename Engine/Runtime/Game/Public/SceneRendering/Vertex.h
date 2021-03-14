// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct Vertex
{
	Vector3 Pos;
	Vector2 TexCoord;
	Vector3 Normal;

	inline Vertex()
	{

	}

	inline Vertex(const Vector3& pos, const Vector3& normal = Vector3::Zero, const Vector2& texCoord = Vector2::Zero)
		: Pos(pos)
		, Normal(normal)
		, TexCoord(texCoord)
	{

	}
};