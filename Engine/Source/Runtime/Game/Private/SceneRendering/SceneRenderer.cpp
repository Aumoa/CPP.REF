// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"
#include "RHI/RHIInterfaces.h"

GENERATE_BODY(SSceneRenderer);

SSceneRenderer::SSceneRenderer() : Super()
{
}

void SSceneRenderer::BeginDraw(const SceneRenderContext& RenderContext)
{
	checkf(!CurrentRenderContext.has_value(), L"BeginDraw called without previous EndDraw call.");
	CurrentRenderContext.emplace(RenderContext);
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