// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

enum class ERHIStrokeDirection
{
	Inner,
	Outer,
	Center
};

enum class ERHIStrokeCapStyle
{
	Flat,
	Square,
	Round,
	Triangle
};

enum class ERHIStrokeJoin
{
	Miter,
	Bevel,
	Round,
	MiterOrBevel,
};

enum class ERHIStrokeDashStyle
{
	Solid,
	Dash,
	Dot,
	DashDot,
	DashDotDot,
	Custom,
};