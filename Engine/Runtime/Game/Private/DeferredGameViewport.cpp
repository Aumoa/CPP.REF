// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DeferredGameViewport.h"

#include "Engine.h"
#include "RHI/RHICommon.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/IRHICommandList.h"
#include "RHI/IRHIResource.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDepthStencilView.h"
#include "RHI/IRHIShaderResourceView.h"
#include "RHI/RHIShaderLibrary.h"
#include "RHI/IRHIMaterialBundle.h"
#include "RHI/IRHIRenderTarget.h"
#include "SceneRendering/DeferredSceneRenderer.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/MinimalViewInfo.h"
#include "SceneRendering/ShaderCameraConstant.h"
#include "SceneRendering/SceneVisibility.h"
#include "SceneRendering/LightSceneProxy.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "Framework/PlayerController.h"
#include "Components/PlayerCameraManager.h"
#include "Logging/LogMacros.h"

DEFINE_STATS_GROUP(DeferredGameViewport);

DeferredGameViewport::DeferredGameViewport() : Super()
{
	
}

DeferredGameViewport::~DeferredGameViewport()
{

}

void DeferredGameViewport::RenderScene(IRHICommandList* inCommandList, Scene* inScene)
{
	DeferredSceneRenderer renderer(inScene);

	{
		QUICK_SCOPED_CYCLE_COUNTER(DeferredGameViewport, CalcVisibility);
		inScene->CalcVisibility();
	}

	{
		inScene->BeginRender(inCommandList);

		QUICK_SCOPED_CYCLE_COUNTER(DeferredGameViewport, PopulateSceneRender);
		inCommandList->BeginRenderTarget(gBuffer.Get());
		renderer.RenderScene(inCommandList);
		inCommandList->EndRenderTarget(gBuffer.Get());

		LightRender(inCommandList, inScene);
		TonemapRender(inCommandList);

		inScene->EndRender(inCommandList);
	}
}

IRHIResource* DeferredGameViewport::GetRenderTarget() const
{
	return renderTarget.Get();
}

void DeferredGameViewport::SetViewportResolution_Internal(int32 x, int32 y)
{
	Super::SetViewportResolution_Internal(x, y);

	if (x == 0 || y == 0) {
		return;
	}

	if (!gBuffer.IsValid)
	{
		RecreateRenderTarget();
	}

	auto device = GEngine.DeviceBundle;

	gBuffer->ResizeBuffers(x, y);
	hdrBuffer->ResizeBuffers(x, y);

	renderTarget = gBuffer->GetRenderTarget(0);
	rtv = device->CreateRenderTargetView(renderTarget.Get());
}

void DeferredGameViewport::LightRender(IRHICommandList* inCommandList, Scene* inScene)
{
	SceneVisibility* localPlayerVisibility = inScene->GetLocalPlayerVisibility();
	const uint64 playerCameraConstantAddr = localPlayerVisibility->ShaderCameraConstants->GetCameraConstantVirtualAddress();

	IRHIDeviceBundle* deviceBundle = GEngine.DeviceBundle;
	IRHIMaterialBundle* materialBundle = GEngine.MaterialBundle;
	RHIShaderLibrary* shaderLibrary = deviceBundle->GetShaderLibrary();

	inCommandList->BeginRenderTarget(hdrBuffer.Get());

	IRHIShader* shader = shaderLibrary->GetShader(RHIShaderLibrary::LightingExperimental);
	inCommandList->SetShader(shader);

	const auto& lights = inScene->LightSceneProxies;
	LightBatch* batch = lights[0]->GetLightBatch();
	inCommandList->SetComputeRootUnorderedAccessView(0, hdrBuffer->GetUnorderedAccessView());
	inCommandList->SetComputeRootConstantBufferView(1, playerCameraConstantAddr);
	inCommandList->SetComputeRootShaderResourceView(2, gBuffer->GetShaderResourceView());
	inCommandList->SetComputeRootConstantBufferView(3, batch->GetLightBuffer()->GetVirtualAddress());
	inCommandList->SetComputeRootShaderResource(4, materialBundle->GetMaterialsBufferVirtualAddress());

	RHIDispatchRaysDesc dispatchRays;
	dispatchRays.Width = hdrBuffer->Width;
	dispatchRays.Height = hdrBuffer->Height;
	inCommandList->DispatchRays(dispatchRays);

	inCommandList->EndRenderTarget(hdrBuffer.Get());
}

void DeferredGameViewport::TonemapRender(IRHICommandList* inCommandList)
{
	IRHIShader* shader = GEngine.DeviceBundle->GetShaderLibrary()->GetShader(RHIShaderLibrary::TonemapShader);
	inCommandList->SetShader(shader);

	IRHIRenderTargetView* rtvs[] = { rtv.Get() };

	inCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::PIXEL_SHADER_RESOURCE, ERHIResourceStates::RENDER_TARGET);
	inCommandList->SetRenderTargets(1, rtvs, nullptr);

	inCommandList->SetGraphicsRootShaderResourceView(0, hdrBuffer->GetShaderResourceView());

	RHIMeshDrawCommand Quad;
	Quad.Topology = ERHIPrimitiveTopology::TriangleStrip;
	Quad.VertexCount = 4;
	inCommandList->DrawMesh(Quad);

	inCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::RENDER_TARGET, ERHIResourceStates::COPY_SOURCE);
}

void DeferredGameViewport::RecreateRenderTarget()
{
	gBuffer = GEngine.DeviceBundle->CreateGBufferRenderTarget();
	hdrBuffer = GEngine.DeviceBundle->CreateHDRRenderTarget();
}