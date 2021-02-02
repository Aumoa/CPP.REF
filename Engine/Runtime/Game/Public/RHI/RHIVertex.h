// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct GAME_API RHIVertex
{
	Vector3 Pos;
	Color Color;
	Vector2 TexCoord;
	Vector3 Normal;

	RHIVertex();
	RHIVertex(const Vector3& pos, const Vector3& normal = Vector3::Zero, const Vector2& texCoord = Vector2::Zero, const ::Color& color = Color::Transparent);
	~RHIVertex();
};