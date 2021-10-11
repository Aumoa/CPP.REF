// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SceneRenderer.h"
#include "SceneView/SceneView.h"
#include "Scene/Scene.h"
#include "RHI/IRHIDeviceContext.h"
#include "Materials/MaterialInterface.h"

SceneRenderer::SceneRenderer(SceneView* InView)
	: _View(InView)
{
}

void SceneRenderer::PopulateCommandLists(IRHIDeviceContext* Context)
{
	SScene* Scene = _View->GetScene();
	uint64 BaseVirtualAddress = _View->GetStructuredBufferViewAddress();

	for (size_t i = 0; i < _View->ViewIndexes.size(); ++i)
	{
		SceneView::PrimitiveViewInfo& ViewInfo = _View->ViewIndexes[i];
		PrimitiveSceneInfo& PrimitiveInfo = Scene->Primitives[ViewInfo.PrimitiveId];

		for (MeshBatch& Batch : PrimitiveInfo.MeshBatches)
		{
			// Set vertex and index buffer.
			RHIVertexBufferView Vbv = Batch.GetVertexBufferView();
			RHIIndexBufferView Ibv = Batch.GetIndexBufferView();

			Context->IASetPrimitiveTopology(ERHIPrimitiveTopology::TriangleList);
			Context->IASetVertexBuffers(0, std::span(&Vbv, 1));
			Context->IASetIndexBuffer(Ibv);

			for (MeshBatchElement& Elem : Batch.Elements)
			{
				SMaterialInterface* Material = Batch.MaterialSlots[Elem.MaterialSlotIndex];
				Material->SetupCommands(Context);
				Context->DrawIndexedInstanced(Elem.IndexCount, Elem.InstanceCount, Elem.StartIndexLocation, Elem.BaseVertexLocation, 0);
			}
		}
	}
}