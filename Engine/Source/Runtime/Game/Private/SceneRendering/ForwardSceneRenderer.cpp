// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/ForwardSceneRenderer.h"
#include "SceneRendering/SceneView.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/MeshBatch.h"
#include "RHI/IRHIDeviceContext.h"
#include "Materials/MaterialInterface.h"
#include "Scene/PrimitiveSceneProxy.h"

ForwardSceneRenderer::ForwardSceneRenderer(SceneRenderTarget& InRenderTarget, SScene* InScene) : Super(InRenderTarget)
	, Scene(InScene)
{
}

void ForwardSceneRenderer::InitViews(std::span<const SceneViewScope> InViews)
{
	Views.reserve(InViews.size());

	for (size_t i = 0; i < InViews.size(); ++i)
	{
		SceneView& View = Views.emplace_back(Scene);
		View.InitViews(InViews[i]);
	}
}

void ForwardSceneRenderer::OnPopulateCommandLists(IRHIDeviceContext* Context)
{
	if (Scene)
	{
		const std::vector<PrimitiveSceneProxy*>& Primitives = Scene->GetPrimitives_RenderThread();

		// Render for each views.
		for (auto& View : Views)
		{
			uint64 BaseVirtualAddress = View.GetStructuredBufferViewAddress();

			// Render view elements.
			for (size_t i = 0; i < View.ViewIndexes.size(); ++i)
			{
				SceneView::PrimitiveViewInfo& ViewInfo = View.ViewIndexes[i];
				const PrimitiveSceneProxy& PrimitiveInfo = *Primitives[ViewInfo.PrimitiveId];

				Context->SetGraphicsRootShaderResourceView(0, BaseVirtualAddress);

				for (const MeshBatch& Batch : PrimitiveInfo.MeshBatches)
				{
					// Set vertex and index buffer.
					//RHIVertexBufferView Vbv = Batch.GetVertexBufferView();
					//RHIIndexBufferView Ibv = Batch.GetIndexBufferView();

					//Context->IASetPrimitiveTopology(ERHIPrimitiveTopology::TriangleList);
					//Context->IASetVertexBuffers(0, std::span(&Vbv, 1));
					//Context->IASetIndexBuffer(Ibv);

					//for (const MeshBatchElement& Elem : Batch.Elements)
					//{
					//	SMaterialInterface* Material = Batch.MaterialSlots[Elem.MaterialSlotIndex];
					//	Material->SetupCommands(Context);
					//	Context->DrawIndexedInstanced(Elem.IndexCount, Elem.InstanceCount, Elem.StartIndexLocation, Elem.BaseVertexLocation, 0);
					//}
				}

				View.AdvanceViewAddress(BaseVirtualAddress, 1);
			}
		}
	}
}