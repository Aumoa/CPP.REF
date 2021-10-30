// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Image/Image.h"
#include "Draw/SlateWindowElementList.h"
#include "AppEngine.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHISolidColorBrush.h"
#include "Draw/PaintArgs.h"

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

	IRHIDevice* Device = GEngine->GetDevice();
	if (Brush.ImageSource)
	{
		checkf(false, L"NOT IMPLEMENTED");
	}
	else
	{
		BrushObject = Device->CreateSolidColorBrush(Brush.TintColor);
		BrushObject->SetOuter(this);
	}
}

SlateBrush SImage::GetBrush()
{
	return Brush;
}

DEFINE_SLATE_CONSTRUCTOR(SImage, attr)
{
	Super::Construct(attr);
	SetBrush(attr._Brush);
}

int32 SImage::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled)
{
	if (BrushObject)
	{
		BrushObject->SetOpacity(Args.RenderOpacity * GetRenderOpacity());
	}

	SlateDrawElement& Element = InDrawElements.AddUninitialized();
	Element.Brush = BrushObject;
	Element.Layer = InLayer;
	Element.Transform = AllottedGeometry.ToPaintGeometry();
	return InLayer;
}