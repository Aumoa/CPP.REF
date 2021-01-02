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
	, primitives_size(0)
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
	primitives_size = 0;

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

void SceneRenderer::AddPrimitives(const vector<PrimitiveSceneProxy*>& sceneProxies)
{
	size_t numPrims = primitives_size;
	size_t newSize = numPrims + sceneProxies.size();

	if (primitives.size() < newSize)
	{
		primitives.resize(newSize);
	}

	for (size_t i = 0; i < sceneProxies.size(); ++i)
	{
		size_t dest = i + numPrims;
		primitives[dest] = sceneProxies[i];
	}

	primitives_size = newSize;
}

PrimitiveSceneProxy* const* SceneRenderer::GetPrimitives() const
{
	return primitives.data();
}

size_t SceneRenderer::GetPrimitiveCount() const
{
	return primitives_size;
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

void SceneRenderer::Application_OnPostSized(int32 width, int32 height)
{
	finalColor = deviceBundle->CreateTexture2D(ERHITextureFormat::B8G8R8A8_UNORM, width, height, ERHIResourceStates::COPY_SOURCE, ERHIResourceFlags::AllowRenderTarget);
	rtv = deviceBundle->CreateRenderTargetView(finalColor.Get());

	sceneSizeX = width;
	sceneSizeY = height;
}