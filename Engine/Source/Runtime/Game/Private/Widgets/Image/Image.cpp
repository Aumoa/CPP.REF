// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Image/Image.h"
#include "Draw/SlateWindowElementList.h"

SImage::SImage(const std::wstring& name) : Super(name)
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

int32 SImage::OnPaint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled)
{
	SlateDrawElement element;
	element.Brush = _brush;
	element.Layer = layer;
	element.Transform = allottedGeometry.ToPaintGeometry();

	drawElements->Add(element);
	return layer;
}