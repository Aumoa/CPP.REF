// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "SlateBrush.h"
#include "Layout/PaintGeometry.h"

struct SlateDrawElement
{
	SlateBrush Brush;
	PaintGeometry Transform;
	int32 Layer = 0;

	SlateDrawElement()
	{
	}

	SlateDrawElement(const SlateBrush& brush, const PaintGeometry& transform, int32 layer = 0)
		: Brush(brush)
		, Transform(transform)
		, Layer(layer)
	{
	}
};