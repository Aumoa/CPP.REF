// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/MatrixInterface/Matrix4x4.h"
#include "Rendering/RenderingMacros.h"

namespace Ayla
{
	struct ALIGNAS_CBUFFER CameraBuffer
	{
		Matrix4x4F ViewProjection;
	};
}