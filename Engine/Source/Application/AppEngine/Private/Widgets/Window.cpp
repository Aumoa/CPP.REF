// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Window.h"
#include "Draw/PaintArgs.h"
#include "Draw/SlateWindowElementList.h"
#include "RHI/IRHIDevice.h"

SWindow::SWindow() : Super()
{
}

void SWindow::TickAndPaint(float InDeltaTime, IRHIDevice* Device)
{
	Vector2 DesiredSize = GetDesiredSize();
	Rect CullingRect = Rect(0, 0, DesiredSize.X, DesiredSize.Y);
	Geometry AllottedGeometry = Geometry::MakeRoot(DesiredSize, SlateLayoutTransform::Identity(), SlateRenderTransform::Identity());

	Tick(AllottedGeometry, InDeltaTime);

	auto Elements = SlateWindowElementList(this);
	Paint(PaintArgs(this, InDeltaTime), AllottedGeometry, CullingRect, Elements, 0, true);
}