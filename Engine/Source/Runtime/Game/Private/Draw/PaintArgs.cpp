// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/PaintArgs.h"

PaintArgs::PaintArgs(SWidget* InParentWidget, float InDeltaTime, IRHIDeviceContext* InDeviceContext, SceneRenderTarget* RT)
	: Parent(InParentWidget)
	, DeltaTime(InDeltaTime)
	, DeviceContext(InDeviceContext)
	, RenderTarget(RT)
{
}

PaintArgs PaintArgs::WithNewParent(SWidget* InParentWidget) const
{
	return PaintArgs(InParentWidget, DeltaTime, DeviceContext, RenderTarget);
}

PaintArgs PaintArgs::WithNewRenderTarget(SceneRenderTarget* RT) const
{
	return PaintArgs(Parent, DeltaTime, DeviceContext, RT);
}