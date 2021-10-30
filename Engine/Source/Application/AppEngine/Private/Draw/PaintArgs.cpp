// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/PaintArgs.h"
#include "Widgets/Widget.h"

PaintArgs::PaintArgs(SWidget* InParentWidget, float InDeltaTime, float InRenderOpacity)
	: Parent(InParentWidget)
	, DeltaTime(InDeltaTime)
	, RenderOpacity(InRenderOpacity)
{
}

PaintArgs PaintArgs::WithNewParent(SWidget* InParentWidget) const
{
	return PaintArgs(InParentWidget, DeltaTime, RenderOpacity * InParentWidget->GetRenderOpacity());
}