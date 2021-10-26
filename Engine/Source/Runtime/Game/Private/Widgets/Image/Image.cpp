// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Image/Image.h"
#include "Draw/SlateWindowElementList.h"

SImage::SImage() : Super()
{
	SetRenderTransform(SlateRenderTransform::Identity());
}

SImage::~SImage()
{
}

Vector2 SImage::GetDesiredSize()
{
	return _brush.ImageSize;
}

void SImage::SetBrush(const SlateBrush& brush)
{
	_brush = brush;
}

SlateBrush SImage::GetBrush()
{
	return _brush;
}

DEFINE_SLATE_CONSTRUCTOR(SImage, attr)
{
	Super::Construct(attr);
	SetBrush(attr._Brush);
}

int32 SImage::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled)
{
	SlateDrawElement& Element = InDrawElements.AddUninitialized();
	Element.Brush = _brush;
	Element.Layer = InLayer;
	Element.Transform = AllottedGeometry.ToPaintGeometry();
	return InLayer;
}