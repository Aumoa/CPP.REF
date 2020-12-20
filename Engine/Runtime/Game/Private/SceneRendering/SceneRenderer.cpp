// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"

#include "RHI/IRHIResource.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/RHITextureFormat.h"
#include "RHI/RHIResourceStates.h"
#include "RHI/RHIResourceFlags.h"
#include "RHI/IRHIDeferredCommandList.h"
#include "Application.h"

using namespace SC::Runtime::Game::RHI;
using namespace SC::Runtime::Game::SceneRendering;

SceneRenderer::SceneRenderer(IRHIDeviceBundle* deviceBundle) : Super()
	, deviceBundle(deviceBundle)
{
	commandList = deviceBundle->CreateDeferredCommandList();

	GApplication.PostSized += bind_delegate(Application_OnPostSized);
}

SceneRenderer::~SceneRenderer()
{

}

void SceneRenderer::BeginRender()
{
	commandList->BeginCommand();
}

void SceneRenderer::EndRender()
{
	commandList->EndCommand();
}

void SceneRenderer::Application_OnPostSized(int32 width, int32 height)
{
	finalColor = deviceBundle->CreateTexture2D(RHITextureFormat::R8G8B8A8_UNORM, width, height, RHIResourceStates::COPY_SOURCE, RHIResourceFlags::AllowRenderTarget);
}