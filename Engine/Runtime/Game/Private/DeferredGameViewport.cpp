// Copyright 2020 Aumoa.lib. All right reserved.

#include "DeferredGameViewport.h"

DeferredGameViewport::DeferredGameViewport() : Super()
{

}

DeferredGameViewport::~DeferredGameViewport()
{

}

void DeferredGameViewport::RenderScene(IRHICommandList* inCommandList, Scene* inScene)
{
	Super::RenderScene(inCommandList, inScene);
}

void DeferredGameViewport::SetViewportResolution_Internal(int32 x, int32 y)
{
	Super::SetViewportResolution_Internal(x, y);

	if (x == 0 || y == 0)
	{
		return;
	}
}