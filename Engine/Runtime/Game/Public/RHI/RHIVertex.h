// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct GAME_API RHIVertex
{
	Vector3 Pos;
	Vector2 TexCoord;
	Vector3 Normal;

	RHIVertex();
	RHIVertex(const Vector3& pos, const Vector3& normal = Vector3::Zero, const Vector2& texCoord = Vector2::Zero);
	~RHIVertex();
};