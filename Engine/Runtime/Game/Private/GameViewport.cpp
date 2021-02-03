// Copyright 2020 Aumoa.lib. All right reserved.

#include "GameViewport.h"

#include "Engine.h"
#include "RHI/IRHIDeviceBundle.h"
#include "RHI/RHITextureFormat.h"
#include "RHI/RHIResourceStates.h"
#include "RHI/RHIResourceFlags.h"
#include "RHI/IRHIResource.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHICommandList.h"
#include "RHI/RHIViewport.h"
#include "RHI/IRHIDepthStencilView.h"
#include "RHI/RHITextureClearValue.h"
#include "Logging/LogMacros.h"
#include "SceneRendering/SceneRenderer.h"
#include "Diagnostics/ScopedCycleCounter.h"

using namespace std;

DEFINE_STATS_GROUP(GameViewport);

GameViewport::GameViewport() : Super()
	, resX(0)
	, resY(0)
{

}

GameViewport::~GameViewport()
{

}

void GameViewport::RenderScene(IRHICommandList* inCommandList, Scene* inScene)
{
	SceneRenderer renderer(inScene);

	{
		QUICK_SCOPED_CYCLE_COUNTER(GameViewport, CalcVisibility);
		renderer.CalcLocalPlayerVisibility();
	}

	{
		QUICK_SCOPED_CYCLE_COUNTER(GameViewport, PopulateSceneRender);
		BeginRender(inCommandList);
		renderer.RenderScene(inCommandList);
		EndRender(inCommandList);
	}
}

IRHIResource* GameViewport::GetRenderTarget() const
{
	return renderTarget.Get();
}

IRHIResource* GameViewport::GetDepthStencilBuffer() const
{
	return depthStencilBuffer.Get();
}

int32 GameViewport::ResolutionX_get() const
{
	return resX;
}

int32 GameViewport::ResolutionY_get() const
{
	return resY;
}

void GameViewport::SetViewportResolution_Internal(int32 x, int32 y)
{
	if (x == 0 || y == 0)
	{
		return;
	}

	resX = x;
	resY = y;

	RHITextureClearValue colorClear(ERHITextureFormat::B8G8R8A8_UNORM);
	colorClear.Color = Color::Transparent;

	RHITextureClearValue depthStencilClear(ERHITextureFormat::D24_UNORM_S8_UINT);
	depthStencilClear.Depth = 1.0f;
	depthStencilClear.Stencil = 0;

	renderTarget = GEngine.DeviceBundle->CreateTexture2D(
		ERHITextureFormat::B8G8R8A8_UNORM,
		resX,
		resY,
		ERHIResourceStates::COPY_SOURCE,
		ERHIResourceFlags::AllowRenderTarget,
		colorClear
	);

	depthStencilBuffer = GEngine.DeviceBundle->CreateTexture2D(
		ERHITextureFormat::R24G8_TYPELESS,
		resX,
		resY,
		ERHIResourceStates::DEPTH_WRITE,
		ERHIResourceFlags::AllowDepthStencil,
		depthStencilClear
	);

	renderTargetView = GEngine.DeviceBundle->CreateRenderTargetView(
		renderTarget.Get()
	);

	depthStencilView = GEngine.DeviceBundle->CreateDepthStencilView(
		depthStencilBuffer.Get(),
		ERHITextureFormat::D24_UNORM_S8_UINT
	);
}

void GameViewport::BeginRender(IRHICommandList* inCommandList)
{
	IRHIRenderTargetView* rtvs[] = { renderTargetView.Get() };

	inCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::COPY_SOURCE, ERHIResourceStates::RENDER_TARGET);
	inCommandList->SetRenderTargets(1, rtvs, depthStencilView.Get());
	inCommandList->SetViewports(RHIViewport(resX, resY));
	inCommandList->SetScissorRects(Rect(0.0f, Vector2((float)resX, (float)resY)));

	inCommandList->ClearRenderTargetView(renderTargetView.Get());
	inCommandList->ClearDepthStencilView(depthStencilView.Get(), 1.0f, 0);
}

void GameViewport::EndRender(IRHICommandList* inCommandList)
{
	inCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::RENDER_TARGET, ERHIResourceStates::COPY_SOURCE);
}