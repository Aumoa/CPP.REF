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

using namespace std;

SceneRenderer::SceneRenderer(IRHIDeviceBundle* deviceBundle) : Super()
	, deviceBundle(deviceBundle)

	, sceneSizeX(0)
	, sceneSizeY(0)
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
	commandList->ResourceTransition(finalColor.Get(), ERHIResourceStates::COPY_SOURCE, ERHIResourceStates::RENDER_TARGET);
	commandList->ClearRenderTargetView(rtv.Get());
}

void SceneRenderer::EndRender()
{
	commandList->ResourceTransition(finalColor.Get(), ERHIResourceStates::RENDER_TARGET, ERHIResourceStates::COPY_SOURCE);
	commandList->EndCommand();
}

void SceneRenderer::PopulateRenderCommands()
{

}

void SceneRenderer::SetPrimitivesArray(span<PrimitiveSceneProxy* const> sceneProxies)
{
	primitives = sceneProxies;
	visibility.resize(primitives.size());
}

void SceneRenderer::AddPrimitive(size_t index)
{
	visibility[index] = true;
}

void SceneRenderer::RemovePrimitive(size_t index)
{
	visibility[index] = false;
}

IRHIDeferredCommandList* SceneRenderer::CommandList_get() const
{
	return commandList.Get();
}

IRHIResource* SceneRenderer::FinalColor_get() const
{
	return finalColor.Get();
}

IRHIRenderTargetView* SceneRenderer::GetFinalColorRTV() const
{
	return rtv.Get();
}

void SceneRenderer::GetSceneSize(int32& x, int32& y)
{
	x = sceneSizeX;
	y = sceneSizeY;
}

span<PrimitiveSceneProxy* const> SceneRenderer::GetPrimitivesView() const
{
	return primitives;
}

void SceneRenderer::Application_OnPostSized(int32 width, int32 height)
{
	finalColor = deviceBundle->CreateTexture2D(ERHITextureFormat::B8G8R8A8_UNORM, width, height, ERHIResourceStates::COPY_SOURCE, ERHIResourceFlags::AllowRenderTarget);
	rtv = deviceBundle->CreateRenderTargetView(finalColor.Get());

	sceneSizeX = width;
	sceneSizeY = height;
}