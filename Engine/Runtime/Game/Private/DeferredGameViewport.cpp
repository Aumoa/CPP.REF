// Copyright 2020 Aumoa.lib. All right reserved.

#include "DeferredGameViewport.h"

#include "Engine.h"
#include "RHI/RHICommon.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/IRHICommandList.h"
#include "RHI/IRHIResource.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDepthStencilView.h"
#include "SceneRendering/DeferredSceneRenderer.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/MinimalViewInfo.h"
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

		APlayerController* localPlayer = inScene->LocalPlayer;
		PlayerCameraManager* cameraManager = localPlayer->CameraManager;

		if (cameraManager == nullptr)
		{
			SE_LOG(LogRendering, Error, L"PlayerController have not a camera manager component. Abort.");
			return;
		}

		MinimalViewInfo viewInfo;
		cameraManager->CalcCameraView(viewInfo);

		renderer.AddViewInfo(viewInfo);
		renderer.CalcVisibility();
	}

	{
		QUICK_SCOPED_CYCLE_COUNTER(DeferredGameViewport, PopulateSceneRender);
		BeginRender(inCommandList);
		renderer.RenderScene(inCommandList);
		EndRender(inCommandList);
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

	RHITextureClearValue clearColor(ERHITextureFormat::B8G8R8A8_UNORM);
	clearColor.Color = Color::Transparent;

	RHITextureClearValue depthStencilClear(ERHITextureFormat::D24_UNORM_S8_UINT);
	depthStencilClear.Depth = 1.0f;
	depthStencilClear.Stencil = 0;

	renderTarget = GEngine.DeviceBundle->CreateTexture2D(
		ERHITextureFormat::B8G8R8A8_UNORM,
		x,
		y,
		ERHIResourceStates::COPY_SOURCE,
		ERHIResourceFlags::AllowRenderTarget,
		clearColor
	);

	clearColor.Format = ERHITextureFormat::R16G16B16A16_FLOAT;
	normalBuffer = GEngine.DeviceBundle->CreateTexture2D(
		ERHITextureFormat::R16G16B16A16_FLOAT,
		x,
		y,
		ERHIResourceStates::PIXEL_SHADER_RESOURCE,
		ERHIResourceFlags::AllowRenderTarget,
		clearColor
	);

	depthStencilBuffer = GEngine.DeviceBundle->CreateTexture2D(
		ERHITextureFormat::R24G8_TYPELESS,
		x,
		y,
		ERHIResourceStates::DEPTH_WRITE,
		ERHIResourceFlags::AllowDepthStencil,
		depthStencilClear
	);

	renderTargetView = GEngine.DeviceBundle->CreateRenderTargetView(
		renderTarget.Get()
	);

	normalBufferView = GEngine.DeviceBundle->CreateRenderTargetView(
		normalBuffer.Get()
	);

	depthStencilView = GEngine.DeviceBundle->CreateDepthStencilView(
		depthStencilBuffer.Get(),
		ERHITextureFormat::D24_UNORM_S8_UINT
	);

	viewport = RHIViewport(x, y);
	scissor = Rect(0.0f, Vector2((float)x, (float)y));
}

#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
void DeferredGameViewport::BeginRender(IRHICommandList* inCommandList)
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

void DeferredGameViewport::EndRender(IRHICommandList* inCommandList)
{
	inCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::RENDER_TARGET, ERHIResourceStates::COPY_SOURCE);
	inCommandList->ResourceTransition(normalBuffer.Get(), ERHIResourceStates::RENDER_TARGET, ERHIResourceStates::PIXEL_SHADER_RESOURCE);
}