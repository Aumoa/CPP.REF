// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Widgets/Images/SImage.h"
#include "RHI/RHIGlobal.h"
#include "RHI/RHIGraphics.h"

SImage::SImage()
{
}

DEFINE_SLATE_CONSTRUCTOR(SImage, Args)
{
	Brush = std::move(Args._Brush);
}

Vector2 SImage::ComputeDesiredSize() const
{
	return Brush.DrawSize;
}

int32 SImage::OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const
{
	PLATFORM_UNREFERENCED_PARAMETER(CullingRect);
	PLATFORM_UNREFERENCED_PARAMETER(bParentEnabled);

	NSlateRenderElement& Element = OutDrawElements.Add();
	Element.AbsolutePosition = AllottedGeometry.GetAbsolutePosition();
	Element.Layout = AllottedGeometry.ToPaintGeometry();

	Element.Layer = InLayer;
	Element.TintColor = Brush.TintColor;
	Element.RenderOpacity = Args.RenderOpacity;

	Element.Proxy = Brush.RenderProxy;
	return InLayer;
}