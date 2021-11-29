// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/PaintArgs.h"
#include "Widgets/Widget.h"

PaintArgs::PaintArgs(SWidget* InParentWidget, float InDeltaTime, float RenderOpacity)
	: Parent(InParentWidget)
	, DeltaTime(InDeltaTime)
	, RenderOpacity(RenderOpacity * (InParentWidget ? InParentWidget->GetRenderOpacity() : 1.0f))
{
}

PaintArgs PaintArgs::WithNewParent(SWidget* InParentWidget) const
{
	return PaintArgs(InParentWidget, DeltaTime, RenderOpacity);
}