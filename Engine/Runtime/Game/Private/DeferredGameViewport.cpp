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
#include "SceneRendering/DeferredSceneRenderer.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/MinimalViewInfo.h"
#include "SceneRendering/ShaderCameraConstant.h"
#include "SceneRendering/SceneVisibility.h"
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
		QUICK_SCOPED_CYCLE_COUNTER(DeferredGameViewport, PopulateSceneRender);
		BeginGeometryRender(inCommandList);
		renderer.RenderScene(inCommandList);
		EndGeometryRender(inCommandList);

		LightRender(inCommandList, inScene);
		TonemapRender(inCommandList);
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

	IRHIDeviceBundle* device = GEngine.DeviceBundle;

	RHITextureClearValue clearColor(ERHITextureFormat::B8G8R8A8_UNORM);
	clearColor.Color = Color::Transparent;

	RHITextureClearValue depthStencilClear(ERHITextureFormat::D24_UNORM_S8_UINT);
	depthStencilClear.Depth = 1.0f;
	depthStencilClear.Stencil = 0;

	renderTarget = device->CreateTexture2D(
		ERHITextureFormat::B8G8R8A8_UNORM,
		x,
		y,
		ERHIResourceStates::COPY_SOURCE,
		ERHIResourceFlags::AllowRenderTarget,
		clearColor
	);

	clearColor.Format = ERHITextureFormat::R16G16B16A16_FLOAT;
	normalBuffer = device->CreateTexture2D(
		ERHITextureFormat::R16G16B16A16_FLOAT,
		x,
		y,
		ERHIResourceStates::PIXEL_SHADER_RESOURCE,
		ERHIResourceFlags::AllowRenderTarget,
		clearColor
	);

	depthStencilBuffer = device->CreateTexture2D(
		ERHITextureFormat::R24G8_TYPELESS,
		x,
		y,
		ERHIResourceStates::DEPTH_WRITE,
		ERHIResourceFlags::AllowDepthStencil,
		depthStencilClear
	);

	clearColor.Format = ERHITextureFormat::R16G16B16A16_FLOAT;
	hdrBuffer = device->CreateTexture2D(
		ERHITextureFormat::R16G16B16A16_FLOAT,
		x,
		y,
		ERHIResourceStates::PIXEL_SHADER_RESOURCE,
		ERHIResourceFlags::AllowRenderTarget,
		clearColor
	);

	renderTargetView = device->CreateRenderTargetView(renderTarget.Get());
	normalBufferView = device->CreateRenderTargetView(normalBuffer.Get());
	depthStencilView = device->CreateDepthStencilView(depthStencilBuffer.Get(), ERHITextureFormat::D24_UNORM_S8_UINT);

	hdrTargetView = device->CreateRenderTargetView(hdrBuffer.Get());
	colorBufferSRV = device->CreateTextureView(renderTarget.Get(), ERHITextureFormat::B8G8R8A8_UNORM);
	normalBufferSRV = device->CreateTextureView(normalBuffer.Get(), ERHITextureFormat::R16G16B16A16_FLOAT);
	depthBufferSRV = device->CreateTextureView(depthStencilBuffer.Get(), ERHITextureFormat::R24_UNORM_X8_TYPELESS);
	hdrBufferSRV = device->CreateTextureView(hdrBuffer.Get(), ERHITextureFormat::R16G16B16A16_FLOAT);

	viewport = RHIViewport(x, y);
	scissor = Rect(0.0f, Vector2((float)x, (float)y));
}

#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
void DeferredGameViewport::BeginGeometryRender(IRHICommandList* inCommandList)
{
	IRHIRenderTargetView* rtvs[] = { renderTargetView.Get(), normalBufferView.Get() };

	inCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::COPY_SOURCE, ERHIResourceStates::RENDER_TARGET);
	inCommandList->ResourceTransition(normalBuffer.Get(), ERHIResourceStates::PIXEL_SHADER_RESOURCE, ERHIResourceStates::RENDER_TARGET);

	inCommandList->SetRenderTargets(ARRAYSIZE(rtvs), rtvs, depthStencilView.Get());
	inCommandList->SetViewports(viewport);
	inCommandList->SetScissorRects(scissor);

	inCommandList->ClearRenderTargetView(renderTargetView.Get());
	inCommandList->ClearDepthStencilView(depthStencilView.Get(), 1.0f, 0);
}

void DeferredGameViewport::EndGeometryRender(IRHICommandList* inCommandList)
{
	inCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::RENDER_TARGET, ERHIResourceStates::PIXEL_SHADER_RESOURCE);
	inCommandList->ResourceTransition(normalBuffer.Get(), ERHIResourceStates::RENDER_TARGET, ERHIResourceStates::PIXEL_SHADER_RESOURCE);
}

void DeferredGameViewport::LightRender(IRHICommandList* inCommandList, Scene* inScene)
{
	IRHIShader* shader = GEngine.DeviceBundle->GetShaderLibrary()->GetShader(RHIShaderLibrary::LightingShader);
	inCommandList->SetShader(shader);

	SceneVisibility* localPlayerVisibility = inScene->GetLocalPlayerVisibility();
	const uint64 playerCameraConstantAddr = localPlayerVisibility->ShaderCameraConstants->GetCameraConstantVirtualAddress();

	IRHIRenderTargetView* rtvs[] = { hdrTargetView.Get() };

	inCommandList->ResourceTransition(hdrBuffer.Get(), ERHIResourceStates::PIXEL_SHADER_RESOURCE, ERHIResourceStates::RENDER_TARGET);

	inCommandList->SetRenderTargets(ARRAYSIZE(rtvs), rtvs, nullptr);
	inCommandList->SetViewports(viewport);
	inCommandList->SetScissorRects(scissor);

	inCommandList->ClearRenderTargetView(hdrTargetView.Get());

	inCommandList->SetGraphicsRootConstantBufferView(0, playerCameraConstantAddr);
	inCommandList->SetGraphicsRootShaderResourceView(1, colorBufferSRV.Get());
	inCommandList->SetGraphicsRootShaderResourceView(2, normalBufferSRV.Get());
	inCommandList->SetGraphicsRootShaderResourceView(3, depthBufferSRV.Get());

	RHIMeshDrawCommand Quad;
	Quad.Topology = ERHIPrimitiveTopology::TriangleStrip;
	Quad.VertexCount = 4;
	inCommandList->DrawMesh(Quad);

	inCommandList->ResourceTransition(hdrBuffer.Get(), ERHIResourceStates::RENDER_TARGET, ERHIResourceStates::PIXEL_SHADER_RESOURCE);
}

void DeferredGameViewport::TonemapRender(IRHICommandList* inCommandList)
{
	IRHIShader* shader = GEngine.DeviceBundle->GetShaderLibrary()->GetShader(RHIShaderLibrary::TonemapShader);
	inCommandList->SetShader(shader);

	IRHIRenderTargetView* rtvs[] = { renderTargetView.Get() };

	inCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::PIXEL_SHADER_RESOURCE, ERHIResourceStates::RENDER_TARGET);

	inCommandList->SetRenderTargets(ARRAYSIZE(rtvs), rtvs, nullptr);
	inCommandList->SetViewports(viewport);
	inCommandList->SetScissorRects(scissor);

	inCommandList->ClearRenderTargetView(renderTargetView.Get());

	inCommandList->SetGraphicsRootShaderResourceView(0, hdrBufferSRV.Get());

	RHIMeshDrawCommand Quad;
	Quad.Topology = ERHIPrimitiveTopology::TriangleStrip;
	Quad.VertexCount = 4;
	inCommandList->DrawMesh(Quad);

	inCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::RENDER_TARGET, ERHIResourceStates::COMMON);
}