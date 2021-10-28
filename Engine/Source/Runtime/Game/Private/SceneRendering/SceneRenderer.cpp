// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"
#include "RHI/IRHIDeviceContext.h"
#include "RHI/IRHITexture2D.h"
#include "SceneRendering/ForwardSceneRenderer.h"

SceneRenderer::SceneRenderer(SceneRenderTarget& InRenderTarget)
	: RenderTarget(InRenderTarget)
{
}

void SceneRenderer::PopulateCommandLists(IRHIDeviceContext* Context)
{
	const bool bShouldBeTransition = RenderTarget.RTTexture && RenderTarget.InitState != ERHIResourceStates::RenderTarget && !RenderTarget.bHasBeenTransited;

	// Transition resource state to render target.
	RHIResourceTransitionBarrier TransitionBarrier = {};
	if (bShouldBeTransition)
	{
		TransitionBarrier.pResource = RenderTarget.RTTexture;
		TransitionBarrier.StateBefore = RenderTarget.InitState;
		TransitionBarrier.StateAfter = ERHIResourceStates::RenderTarget;
		Context->ResourceBarrier(TransitionBarrier);
		RenderTarget.bHasBeenTransited = true;
	}

	// Setup render targets.
	Context->OMSetRenderTargets(RenderTarget.RTV, RenderTarget.IndexOfRTV, 1, RenderTarget.DSV, RenderTarget.IndexOfDSV);
	if (RenderTarget.RTV)
	{
		// TEST IMPLEMENTATION: Clear color is RED.
		if (dynamic_cast<ForwardSceneRenderer*>(this))
		{
			Context->ClearRenderTargetView(RenderTarget.RTV, RenderTarget.IndexOfRTV, NamedColors::Red);
		}
	}
	if (RenderTarget.DSV)
	{
		Context->ClearDepthStencilView(RenderTarget.DSV, RenderTarget.IndexOfDSV, 1.0f, 0);
	}

	// Setup viewports.
	Context->RSSetViewport(RenderTarget.Viewport);
	Context->RSSetScissorRect(RenderTarget.ScissorRect);

	// Populate command lists.
	OnPopulateCommandLists(Context);

	// Finally, transition resource state to initial state.
	if (bShouldBeTransition)
	{
		std::swap(TransitionBarrier.StateBefore, TransitionBarrier.StateAfter);
		Context->ResourceBarrier(TransitionBarrier);
	}
}