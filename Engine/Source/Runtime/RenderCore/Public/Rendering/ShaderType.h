// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShaderType.gen.h"

namespace Ayla
{
	AENUM()
	enum class ShaderType
	{
		Vertex,
		Pixel,
		Domain,
		Hull,
		Geometry,
		Compute,
		RayGeneration,
		ClosestHit,
		Miss
	};
}