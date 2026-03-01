// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	struct PrimitiveVertex
	{
		Vector3F Position;
		Vector3F Normal;
		Vector2F Tex;
	};
}