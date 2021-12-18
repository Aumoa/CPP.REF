// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Image/Image.h"
#include "Draw/SlateWindowElementList.h"
#include "Draw/PaintArgs.h"

GENERATE_BODY(SImage);

SImage::SImage() : Super()
{
	SetRenderTransform(SlateRenderTransform::Identity());
}

SImage::~SImage()
{
}

Vector2 SImage::GetDesiredSize()
{
	return Brush.ImageSize;
}

void SImage::SetBrush(const SlateBrush& InBrush)
{
	Brush = InBrush;
}

SlateBrush SImage::GetBrush()
{
	return Brush;
}

DEFINE_SLATE_CONSTRUCTOR(SImage, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);
	SetBrush(Attr._Brush);
}

int32 SImage::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled)
{
	const PaintArgs NewArgs = Args.WithNewParent(this);
	SlateDrawElement::MakeBox(InDrawElements, Brush, NewArgs.RenderOpacity, AllottedGeometry.ToPaintGeometry(), InLayer);
	return InLayer;
}