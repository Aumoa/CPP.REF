// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/PaintArgs.h"

PaintArgs::PaintArgs(SWidget* InParentWidget, float InDeltaTime, IRHIDeviceContext* InDeviceContext, SceneRenderer* Renderer)
	: Parent(InParentWidget)
	, DeltaTime(InDeltaTime)
	, DeviceContext(InDeviceContext)
	, Renderer(Renderer)
{
}

PaintArgs PaintArgs::WithNewParent(SWidget* InParentWidget) const
{
	return PaintArgs(InParentWidget, DeltaTime, DeviceContext, Renderer);
}