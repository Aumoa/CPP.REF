// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/DeferredSceneRenderer.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game::SceneRendering;

DeferredSceneRenderer::DeferredSceneRenderer(RHI::IRHIDeviceBundle* deviceBundle) : Super(deviceBundle)
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