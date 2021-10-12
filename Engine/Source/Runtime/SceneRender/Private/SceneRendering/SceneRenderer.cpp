// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"
#include "SceneRendering/SceneView.h"
#include "SceneRendering/Scene.h"
#include "RHI/IRHIDeviceContext.h"
#include "RHI/IRHITexture2D.h"
#include "Materials/MaterialInterface.h"

SceneRenderer::SceneRenderer(SScene* InScene) : Scene(InScene)
{
}

void SceneRenderer::InitViews(IRHIDeviceContext* Context, std::span<const SceneViewScope> InViews)
{
	Views.reserve(InViews.size());

	for (size_t i = 0; i < InViews.size(); ++i)
	{
		SceneView& View = Views.emplace_back(Scene);
		View.InitViews(Context, InViews[i]);
	}
}

void SceneRenderer::PopulateCommandLists(IRHIDeviceContext* Context, const SceneRenderTarget& InRenderTarget)
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

	const std::vector<std::optional<PrimitiveSceneInfo>>& Primitives = Scene->GetPrimitives();

	// Render for each views.
	for (auto& View : Views)
	{
		uint64 BaseVirtualAddress = View.GetStructuredBufferViewAddress();

		// Render view elements.
		for (size_t i = 0; i < View.ViewIndexes.size(); ++i)
		{
			SceneView::PrimitiveViewInfo& ViewInfo = View.ViewIndexes[i];
			const PrimitiveSceneInfo& PrimitiveInfo = *Primitives[ViewInfo.PrimitiveId];

			for (const MeshBatch& Batch : PrimitiveInfo.MeshBatches)
			{
				// Set vertex and index buffer.
				RHIVertexBufferView Vbv = Batch.GetVertexBufferView();
				RHIIndexBufferView Ibv = Batch.GetIndexBufferView();

				Context->IASetPrimitiveTopology(ERHIPrimitiveTopology::TriangleList);
				Context->IASetVertexBuffers(0, std::span(&Vbv, 1));
				Context->IASetIndexBuffer(Ibv);

				for (const MeshBatchElement& Elem : Batch.Elements)
				{
					SMaterialInterface* Material = Batch.MaterialSlots[Elem.MaterialSlotIndex];
					Material->SetupCommands(Context);
					Context->DrawIndexedInstanced(Elem.IndexCount, Elem.InstanceCount, Elem.StartIndexLocation, Elem.BaseVertexLocation, 0);
				}
			}

			View.AdvanceViewAddress(BaseVirtualAddress, 1);
		}
	}

	// Finally, transition resource state to initial state.
	if (bShouldBeTransition)
	{
		std::swap(TransitionBarrier.StateBefore, TransitionBarrier.StateAfter);
		Context->ResourceBarrier(TransitionBarrier);
	}
}