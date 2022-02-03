// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"
#include "RHI/IRHIDeviceContext.h"
#include "SceneRenderTarget/SceneRenderTargetInterface.h"

GENERATE_BODY(SSceneRenderer);

SSceneRenderer::SSceneRenderer() : Super()
{
}

void SSceneRenderer::BeginDraw(const SceneRenderContext& RenderContext)
{
	checkf(!CurrentRenderContext.has_value(), L"BeginDraw called without previous EndDraw call.");
	CurrentRenderContext.emplace(RenderContext);

	// Transition to render target and clear buffers.
	auto DC = RenderContext.DeviceContext;
	RenderContext.RenderTarget->TransitRenderTargetViews(DC, true);
	RenderContext.RenderTarget->ClearRenderTargetViews(DC);
}

void SSceneRenderer::EndDraw()
{
	checkf(CurrentRenderContext.has_value(), L"EndDraw called without BeginDraw call.");
	CurrentRenderContext.reset();
}

void SSceneRenderer::DrawScene(SScene* Scene)
{
}

const SceneRenderContext& SSceneRenderer::GetCurrentRenderContext()
{
	return *CurrentRenderContext;
}