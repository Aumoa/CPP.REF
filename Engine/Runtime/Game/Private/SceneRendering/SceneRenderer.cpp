// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"

#include "RHI/IRHIResource.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/RHITextureFormat.h"
#include "RHI/RHIResourceStates.h"
#include "RHI/RHIResourceFlags.h"
#include "RHI/IRHIDeferredCommandList.h"
#include "RHI/IRHIRenderTargetView.h"
#include "Application.h"

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
	commandList->ResourceTransition(finalColor.Get(), RHIResourceStates::COPY_SOURCE, RHIResourceStates::RENDER_TARGET);
	commandList->ClearRenderTargetView(rtv.Get());
}

void SceneRenderer::EndRender()
{
	commandList->ResourceTransition(finalColor.Get(), RHIResourceStates::RENDER_TARGET, RHIResourceStates::COPY_SOURCE);
	commandList->EndCommand();
}

IRHIDeferredCommandList* SceneRenderer::CommandList_get() const
{
	return commandList.Get();
}

IRHIResource* SceneRenderer::FinalColor_get() const
{
	return finalColor.Get();
}

void SceneRenderer::Application_OnPostSized(int32 width, int32 height)
{
	finalColor = deviceBundle->CreateTexture2D(RHITextureFormat::R8G8B8A8_UNORM, width, height, RHIResourceStates::COPY_SOURCE, RHIResourceFlags::AllowRenderTarget);
	rtv = deviceBundle->CreateRenderTargetView(finalColor.Get());
}