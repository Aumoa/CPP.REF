// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/DeferredSceneRenderer.h"

#include <span>
#include "Engine.h"
#include "RHI/IRHIDeferredCommandList.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/RHIShaderLibrary.h"
#include "RHI/RHIPrimitiveTopology.h"
#include "RHI/RHIViewport.h"
#include "SceneRendering/PrimitiveSceneProxy.h"

using namespace std;

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

void DeferredSceneRenderer::PopulateRenderCommands()
{
	IRHIRenderTargetView* rtv = GetFinalColorRTV();
	CommandList->SetRenderTargets(1, &rtv);

	int32 sceneX, sceneY;
	GetSceneSize(sceneX, sceneY);

	CommandList->SetScissorRects(Rect(0, 0, sceneX, sceneY));
	CommandList->SetViewports(RHIViewport(sceneX, sceneY));

	TRefPtr<RHIShaderLibrary> shaderLibrary = GEngine.DeviceBundle->GetShaderLibrary();
	IRHIShader* test_shader = shaderLibrary->GetShader(0);

	PrimitiveSceneProxy* const* sceneProxyArray = GetPrimitives();
	auto primitives = span(sceneProxyArray, GetPrimitiveCount());
	for (auto& primitive : primitives)
	{
		// TODO: Render primitives.
	}

	CommandList->SetShader(test_shader);
	CommandList->SetPrimitiveTopology(RHIPrimitiveTopology::TRIANGLELIST);
	CommandList->DrawInstanced(3, 1, 0, 0);
}