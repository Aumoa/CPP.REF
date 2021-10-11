// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Scene/Scene.h"
#include "SceneRendering/SceneRenderer.h"
#include "SceneRendering/SceneRenderTarget.h"
#include "RHI/IRHIDeviceContext.h"
#include "RHI/IRHITexture2D.h"

SScene::SScene(IRHIDevice* InDevice) : Super()
	, _Device(InDevice)
	, _PrimarySceneView(this)
{
}

SScene::~SScene()
{
}

void SScene::InitViews(IRHIDeviceContext* Context, const SceneViewScope& InPrimarySceneView)
{
	_PrimarySceneView.InitViews(Context, InPrimarySceneView);
}

void SScene::PopulateCommandLists(IRHIDeviceContext* Context, const SceneRenderTarget& InRenderTarget)
{
	const bool bShouldBeTransition = InRenderTarget.InitState != ERHIResourceStates::RenderTarget;

	// Transition resource state to render target.
	RHIResourceTransitionBarrier TransitionBarrier = {};
	if (bShouldBeTransition)
	{
		TransitionBarrier.pResource = InRenderTarget.RTTexture;
		TransitionBarrier.StateBefore = InRenderTarget.InitState;
		TransitionBarrier.StateAfter = ERHIResourceStates::RenderTarget;
		Context->ResourceBarrier(TransitionBarrier);
	}

	// Setup render targets.
	Context->OMSetRenderTargets(InRenderTarget.RTV, InRenderTarget.IndexOfRTV, 1, InRenderTarget.DSV, InRenderTarget.IndexOfDSV);
	Context->ClearRenderTargetView(InRenderTarget.RTV, InRenderTarget.IndexOfRTV, NamedColors::Transparent);
	Context->ClearDepthStencilView(InRenderTarget.DSV, InRenderTarget.IndexOfDSV, 1.0f, 0);

	// Setup viewports.
	Context->RSSetViewport(InRenderTarget.Viewport);
	Context->RSSetScissorRect(InRenderTarget.ScissorRect);

	// Render scene with renderers.
	SceneRenderer Renderer(&_PrimarySceneView);
	Renderer.PopulateCommandLists(Context);

	// Finally, transition resource state to initial state.
	if (bShouldBeTransition)
	{
		std::swap(TransitionBarrier.StateBefore, TransitionBarrier.StateAfter);
		Context->ResourceBarrier(TransitionBarrier);
	}
}

IRHIDevice* SScene::GetDevice()
{
	return _Device;
}