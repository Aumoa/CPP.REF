// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/VectorInterface/Color.h"

namespace Ayla
{
	struct PositionColorVertex
	{
		Vector3F Position;
		::Ayla::Color Color;
	};
}