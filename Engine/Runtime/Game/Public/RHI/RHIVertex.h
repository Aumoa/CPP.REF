// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct GAME_API RHIVertex
{
	Vector3 Pos;
	Color Color;

	RHIVertex();
	RHIVertex(const Vector3& pos, const ::Color& color);
	~RHIVertex();
};