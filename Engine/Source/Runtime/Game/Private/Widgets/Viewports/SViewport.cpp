// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Viewports/SViewport.h"
#include "Widgets/Viewports/SWindow.h"

SViewport::SViewport()
	: Super()
{
}

void SViewport::Tick(const Geometry& AllottedGeometry, const TimeSpan& deltaTime)
{
	Super::Tick(AllottedGeometry, deltaTime);
}

int32 SViewport::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
	return InLayer;
}

DEFINE_SLATE_CONSTRUCTOR(SViewport, attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(attr);

	attr._Window->SwapChainResized.AddShared(this, &SViewport::OnSwapChainResized_RenderThread);
}

void SViewport::OnSwapChainResized_RenderThread(Vector2N size)
{
}