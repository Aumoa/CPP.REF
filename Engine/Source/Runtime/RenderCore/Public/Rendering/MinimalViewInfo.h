// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"
#include "Numerics/TransformInterface/Quaternion.h"

namespace Ayla
{
	struct MinimalViewInfo
	{
		Vector3D Position;
		QuaternionD Rotation;
		double FieldOfView;
		std::optional<double> AspectRatio;
	};
}