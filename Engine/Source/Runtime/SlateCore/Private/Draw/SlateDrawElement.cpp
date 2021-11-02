// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/SlateDrawElement.h"
#include "Draw/SlateWindowElementList.h"

SlateDrawElement::SlateDrawElement()
{
}

SlateDrawElement& SlateDrawElement::MakeBox(SlateWindowElementList& List, const SlateBrush& InBrush, const PaintGeometry& InTransform, int32 InLayer)
{
	SlateDrawElement& Element = List.AddUninitialized();
	Element.Brush = InBrush;
	Element.Transform = InTransform;
	Element.Layer = InLayer;
	return Element;
}