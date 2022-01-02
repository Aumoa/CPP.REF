// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/PaintArgs.h"
#include "Widgets/Widget.h"

PaintArgs::PaintArgs(IRHIDevice* Device, SWidget* InParentWidget, float InDeltaTime, float RenderOpacity)
	: Device(Device)
	, Parent(InParentWidget)
	, DeltaTime(InDeltaTime)
	, RenderOpacity(RenderOpacity * (InParentWidget ? InParentWidget->GetRenderOpacity() : 1.0f))
{
}

PaintArgs PaintArgs::InitPaintArgs(IRHIDevice* Device, float InDeltaTime)
{
	return PaintArgs(Device, nullptr, InDeltaTime, 1.0f);
}

PaintArgs PaintArgs::WithNewParent(SWidget* InParentWidget) const
{
	return PaintArgs(Device, InParentWidget, DeltaTime, RenderOpacity * InParentWidget->GetRenderOpacity());
}