// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SlateBrush.h"
#include "Layout/Layout.h"

class SlateWindowElementList;

struct APPENGINE_API SlateDrawElement
{
	SlateBrush Brush;
	PaintGeometry Transform;
	int32 Layer = 0;

	SlateDrawElement();

	static SlateDrawElement& MakeBox(SlateWindowElementList& List, const SlateBrush& InBrush, const PaintGeometry& InTransform, int32 InLayer);
};