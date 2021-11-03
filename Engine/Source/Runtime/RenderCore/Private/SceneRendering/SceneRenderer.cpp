// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"
#include "SceneRendering/SceneRenderContext.h"
#include "SceneRenderTarget/SceneRenderTargetInterface.h"
#include "RHI/IRHIDeviceContext.h"
#include "RHI/IRHITexture2D.h"

SceneRenderer::SceneRenderer(SceneRenderContext* RenderContext, ERHIResourceStates InitialState, bool bRestoreTransition)
	: InitialState(InitialState)
	, bRestoreTransition(bRestoreTransition)
	, RenderContext(RenderContext)
{
}

void SceneRenderer::BeginDraw()
{
	RHIResourceTransitionBarrier TransitionBarrier = {};
	TransitionBarrier.pResource = RenderContext->RenderTarget->GetRenderTexture();
	TransitionBarrier.StateBefore = InitialState;
	TransitionBarrier.StateAfter = ERHIResourceStates::RenderTarget;

	RenderContext->DeviceContext->ResourceBarrier(TransitionBarrier);
}

void SceneRenderer::EndDraw()
{
	if (bRestoreTransition)
	{
		RHIResourceTransitionBarrier TransitionBarrier = {};
		TransitionBarrier.pResource = RenderContext->RenderTarget->GetRenderTexture();
		TransitionBarrier.StateBefore = ERHIResourceStates::RenderTarget;
		TransitionBarrier.StateAfter = InitialState;

		RenderContext->DeviceContext->ResourceBarrier(TransitionBarrier);
	}
}