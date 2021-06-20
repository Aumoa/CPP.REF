// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRenderer.h"
#include "GameStructures.h"
#include "Scene.h"
#include "SceneVisibility.h"
#include "PrimitiveSceneProxy.h"
#include "RHI/RHIDeviceContext.h"
#include "RHI/RHIShader.h"
#include "RHI/RHIVertexFactory.h"

SceneRenderer::SceneRenderer(Scene* scene, RHIShader* shader) : Super()
	, _scene(scene)
	, _shader(shader)
{
}

void SceneRenderer::RenderScene(RHIDeviceContext* dc)
{
	dc->SetGraphicsShader(_shader);

	RenderWithSceneVisibility(dc, _scene->_localPlayerView);
}

void SceneRenderer::RenderWithSceneVisibility(RHIDeviceContext* dc, SceneVisibility* view)
{
	view->ForEachVisibleItem([this, dc, view](size_t idx)
	{
		PrimitiveSceneProxy* proxy = _scene->_primitives[idx];
		if (proxy == nullptr)
		{
			return;
		}

		view->SetupView(dc, _shader, idx);
		RenderPrimitive(dc, proxy);
	});
}

void SceneRenderer::RenderPrimitive(RHIDeviceContext* dc, PrimitiveSceneProxy* proxy)
{
	dc->IASetPrimitiveTopology(ERHIPrimitiveTopology::TriangleList);

	for (auto& batch : proxy->MeshBatches)
	{
		uint32 vStride = batch.VertexFactory->GetVertexStride();

		for (auto& element : batch.Elements)
		{
			RHIVertexBufferView vbv;
			vbv.BufferLocation = element.VertexBufferLocation;
			vbv.SizeInBytes = vStride * element.VertexCount;
			vbv.StrideInBytes = vStride;

			RHIIndexBufferView ibv;
			ibv.BufferLocation = element.IndexBufferLocation;
			ibv.SizeInBytes = sizeof(uint32) * element.IndexCount;
			ibv.Format = ERHIVertexElementFormat::R32_UINT;

			dc->IASetVertexBuffers(0, 1, &vbv);
			dc->IASetIndexBuffer(ibv);
			dc->DrawIndexedInstanced(element.IndexCount, 1);
		}
	}
}