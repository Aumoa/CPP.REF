// Copyright 2020 Aumoa.lib. All right reserved.

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
		inCommandList->BeginRenderTarget(gbuffer.Get());
		renderer.RenderScene(inCommandList);
		inCommandList->EndRenderTarget(gbuffer.Get());

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

	if (!gbuffer.IsValid)
	{
		RecreateRenderTarget();
	}

	auto device = GEngine.DeviceBundle;

	gbuffer->ResizeBuffers(x, y);
	renderTarget = gbuffer->GetRenderTarget(0);
	rtv = device->CreateRenderTargetView(renderTarget.Get());

	RHITextureClearValue clearColor(ERHITextureFormat::R16G16B16A16_FLOAT);
	clearColor.Color = Color::Transparent;

	hdrBuffer = device->CreateTexture2D(
		ERHITextureFormat::R16G16B16A16_FLOAT,
		x,
		y,
		ERHIResourceStates::PIXEL_SHADER_RESOURCE,
		ERHIResourceFlags::AllowRenderTarget,
		clearColor
	);

	hdrTargetView = device->CreateRenderTargetView(hdrBuffer.Get());
	hdrBufferSRV = device->CreateTextureView(hdrBuffer.Get(), ERHITextureFormat::R16G16B16A16_FLOAT);

	viewport = RHIViewport(x, y);
	scissor = Rect(0.0f, Vector2((float)x, (float)y));
}

void DeferredGameViewport::LightRender(IRHICommandList* inCommandList, Scene* inScene)
{
	SceneVisibility* localPlayerVisibility = inScene->GetLocalPlayerVisibility();
	const uint64 playerCameraConstantAddr = localPlayerVisibility->ShaderCameraConstants->GetCameraConstantVirtualAddress();

	IRHIMaterialBundle* materialBundle = GEngine.MaterialBundle;

	IRHIRenderTargetView* rtvs[] = { hdrTargetView.Get() };

	inCommandList->ResourceTransition(hdrBuffer.Get(), ERHIResourceStates::PIXEL_SHADER_RESOURCE, ERHIResourceStates::RENDER_TARGET);

	inCommandList->SetRenderTargets(1, rtvs, nullptr);
	inCommandList->SetViewports(viewport);
	inCommandList->SetScissorRects(scissor);

	inCommandList->ClearRenderTargetView(hdrTargetView.Get());

	IRHIShader* shader = GEngine.DeviceBundle->GetShaderLibrary()->GetShader(RHIShaderLibrary::LightingShader);
	inCommandList->SetShader(shader);

	inCommandList->SetGraphicsRootConstantBufferView(0, playerCameraConstantAddr);
	inCommandList->SetGraphicsRootShaderResourceView(1, gbuffer->GetShaderResourceView());
	inCommandList->SetGraphicsRootShaderResource(3, materialBundle->GetMaterialsBufferVirtualAddress());

	RHIMeshDrawCommand Quad;
	Quad.Topology = ERHIPrimitiveTopology::TriangleStrip;
	Quad.VertexCount = 4;

	const auto& lights = inScene->LightSceneProxies;
	for (auto& light : lights)
	{
		LightBatch* batch = light->GetLightBatch();
		inCommandList->SetGraphicsRootConstantBufferView(2, batch->GetLightBuffer()->GetVirtualAddress());

		inCommandList->DrawMesh(Quad);
	}

	shader = GEngine.DeviceBundle->GetShaderLibrary()->GetShader(RHIShaderLibrary::ColorEmplaceShader);
	inCommandList->SetShader(shader);
	inCommandList->DrawMesh(Quad);

	inCommandList->ResourceTransition(hdrBuffer.Get(), ERHIResourceStates::RENDER_TARGET, ERHIResourceStates::PIXEL_SHADER_RESOURCE);
}

void DeferredGameViewport::TonemapRender(IRHICommandList* inCommandList)
{
	IRHIShader* shader = GEngine.DeviceBundle->GetShaderLibrary()->GetShader(RHIShaderLibrary::TonemapShader);
	inCommandList->SetShader(shader);

	IRHIRenderTargetView* rtvs[] = { rtv.Get() };

	inCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::PIXEL_SHADER_RESOURCE, ERHIResourceStates::RENDER_TARGET);

	inCommandList->SetRenderTargets(1, rtvs, nullptr);
	inCommandList->SetViewports(viewport);
	inCommandList->SetScissorRects(scissor);

	inCommandList->ClearRenderTargetView(rtv.Get());

	inCommandList->SetGraphicsRootShaderResourceView(0, hdrBufferSRV.Get());

	RHIMeshDrawCommand Quad;
	Quad.Topology = ERHIPrimitiveTopology::TriangleStrip;
	Quad.VertexCount = 4;
	inCommandList->DrawMesh(Quad);

	inCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::RENDER_TARGET, ERHIResourceStates::COPY_SOURCE);
}

void DeferredGameViewport::RecreateRenderTarget()
{
	gbuffer = GEngine.DeviceBundle->CreateGBufferRenderTarget();
}