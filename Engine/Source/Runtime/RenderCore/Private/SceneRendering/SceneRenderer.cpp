// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"
#include "SceneRendering/SceneRenderContext.h"
#include "SceneRenderTarget/SceneRenderTargetInterface.h"
#include "RHI/IRHIDeviceContext.h"
#include "RHI/IRHITexture2D.h"

SceneRenderer::SceneRenderer(SceneRenderContext* RenderContext, bool bRestoreTransition)
	: bRestoreTransition(bRestoreTransition)
	, RenderContext(RenderContext)
{
}

void SceneRenderer::BeginDraw()
{
	IRHIDeviceContext* Context = RenderContext->DeviceContext;
	SSceneRenderTargetInterface* RenderTarget = RenderContext->RenderTarget;

	RenderTarget->TransitRenderTargetViews(Context, true);
	RenderTarget->ClearRenderTargetViews(Context);
}

void SceneRenderer::EndDraw()
{
	IRHIDeviceContext* Context = RenderContext->DeviceContext;
	SSceneRenderTargetInterface* RenderTarget = RenderContext->RenderTarget;

	if (bRestoreTransition)
	{
		RenderTarget->TransitRenderTargetViews(Context, false);
	}
}