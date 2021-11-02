// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/PaintArgs.h"

PaintArgs::PaintArgs(SWidget* InParentWidget, float InDeltaTime)
	: Parent(InParentWidget)
	, DeltaTime(InDeltaTime)
{
}

PaintArgs PaintArgs::WithNewParent(SWidget* InParentWidget) const
{
	return PaintArgs(InParentWidget, DeltaTime);
}