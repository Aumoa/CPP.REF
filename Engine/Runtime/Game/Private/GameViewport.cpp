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

GameViewport::GameViewport() : Super()
	, resX(0)
	, resY(0)
{

}

GameViewport::~GameViewport()
{

}

void GameViewport::BeginRender(IRHICommandList* immediateCommandList)
{
	IRHIRenderTargetView* rtvs[] = { renderTargetView.Get() };

	immediateCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::COPY_SOURCE, ERHIResourceStates::RENDER_TARGET);
	immediateCommandList->SetRenderTargets(1, rtvs, depthStencilView.Get());
	immediateCommandList->SetViewports(RHIViewport(resX, resY));
	immediateCommandList->SetScissorRects(Rect(0.0f, Vector2((float)resX, (float)resY)));

	immediateCommandList->ClearRenderTargetView(renderTargetView.Get());
}

void GameViewport::EndRender(IRHICommandList* immediateCommandList)
{
	immediateCommandList->ResourceTransition(renderTarget.Get(), ERHIResourceStates::RENDER_TARGET, ERHIResourceStates::COPY_SOURCE);
}

IRHIResource* GameViewport::GetRenderTarget() const
{
	return renderTarget.Get();
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
	resX = x;
	resY = y;

	renderTarget = GEngine.DeviceBundle->CreateTexture2D(
		ERHITextureFormat::B8G8R8A8_UNORM,
		resX,
		resY,
		ERHIResourceStates::COPY_SOURCE,
		ERHIResourceFlags::AllowRenderTarget
	);

	depthStencilBuffer = GEngine.DeviceBundle->CreateTexture2D(
		ERHITextureFormat::D24_UNORM_S8_UINT,
		resX,
		resY,
		ERHIResourceStates::DEPTH_WRITE,
		ERHIResourceFlags::AllowDepthStencil
	);

	renderTargetView = GEngine.DeviceBundle->CreateRenderTargetView(
		renderTarget.Get()
	);

	depthStencilView = GEngine.DeviceBundle->CreateDepthStencilView(
		depthStencilBuffer.Get()
	);
}