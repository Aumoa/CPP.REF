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
		uint32 iStride = batch.VertexFactory->GetIndexStride();

		RHIVertexBufferView vbv;
		vbv.BufferLocation = batch.VertexBufferLocation;
		vbv.SizeInBytes = vStride * (uint32)batch.VertexBuffer.size();
		vbv.StrideInBytes = vStride;

		RHIIndexBufferView ibv;
		ibv.BufferLocation = batch.IndexBufferLocation;
		ibv.SizeInBytes = sizeof(uint32) * (uint32)batch.IndexBuffer.size();
		ibv.Format = ERHIVertexElementFormat::R32_UINT;

		dc->IASetVertexBuffers(0, 1, &vbv);
		dc->IASetIndexBuffer(ibv);

		for (auto& element : batch.Elements)
		{
			dc->DrawIndexedInstanced(element.IndexCount, 1, element.StartIndexLocation, element.BaseVertexLocation);
		}
	}
}