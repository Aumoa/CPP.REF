// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/VectorInterface/Color.h"

struct NSlateBrush
{
	Vector2 DrawSize = Vector2(32.0f, 32.0f);
	Color TintColor = NamedColors::White;
};