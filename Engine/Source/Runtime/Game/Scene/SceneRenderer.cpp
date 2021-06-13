// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Core;
import SC.Runtime.RenderCore;
import SC.Runtime.Game;

SceneRenderer::SceneRenderer(Scene* scene, ColorShader* shader) : Super()
	, _scene(scene)
	, _shader(shader)
{
}

void SceneRenderer::RenderScene(RHIDeviceContext* dc)
{
	dc->SetGraphicsShader(_shader);

	for (size_t i = 0; i < _scene->_primitives.size(); ++i)
	{
		PrimitiveSceneProxy* proxy = _scene->_primitives[i];
		if (proxy == nullptr)
		{
			continue;
		}

		RenderPrimitive(dc, proxy);
	}
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
			dc->DrawIndexedInstanced(3, 1);
		}
	}
}