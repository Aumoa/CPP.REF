// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/DeferredSceneRenderer.h"

DeferredSceneRenderer::DeferredSceneRenderer(IRHIDeviceBundle* deviceBundle) : Super(deviceBundle)
{

}

DeferredSceneRenderer::~DeferredSceneRenderer()
{

}

void DeferredSceneRenderer::BeginRender()
{
	Super::BeginRender();
}

void DeferredSceneRenderer::EndRender()
{
	Super::EndRender();
}