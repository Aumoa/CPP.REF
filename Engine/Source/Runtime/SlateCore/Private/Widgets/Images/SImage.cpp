// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Widgets/Images/SImage.h"

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
	PLATFORM_UNREFERENCED_PARAMETER(Args);
	PLATFORM_UNREFERENCED_PARAMETER(AllottedGeometry);
	PLATFORM_UNREFERENCED_PARAMETER(CullingRect);
	PLATFORM_UNREFERENCED_PARAMETER(InLayer);
	PLATFORM_UNREFERENCED_PARAMETER(bParentEnabled);
	OutDrawElements.Add();
	return InLayer;
}