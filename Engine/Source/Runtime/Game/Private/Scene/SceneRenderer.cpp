// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Scene/SceneRenderer.h"
#include "Scene/Scene.h"
#include "Scene/SceneVisibility.h"
#include "Scene/PrimitiveSceneProxy.h"
#include "GameStructures.h"
#include "Materials/Material.h"

SSceneRenderer::SSceneRenderer(SScene* scene, SRHIShader* shader) : Super()
	, _scene(scene)
	, _shader(shader)
{
}

SSceneRenderer::SSceneRenderer(SSceneRenderer&& rhs) noexcept : Super()
	, _scene(rhs._scene)
	, _shader(rhs._shader)
	, _visibility(rhs._visibility)
	, _drawRelevances(move(rhs._drawRelevances))
	, _relevances(rhs._relevances)
{
}

SSceneRenderer::~SSceneRenderer()
{
}

void SSceneRenderer::CollectPrimitives(SSceneVisibility* view)
{
	size_t primitives = _scene->_primitives.size();
	_drawRelevances.resize(std::max(primitives, _drawRelevances.size()));
	_relevances = 0;

	view->ForEachVisibleItem([&](size_t idx, size_t viewIndex)
	{
		SPrimitiveSceneProxy* sceneProxy = _scene->_primitives[idx];
		
		for (auto& batch : sceneProxy->MeshBatches)
		{
			bool bCollectBatch = false;
			MeshBatchDrawRelevance* relevance = nullptr;

			for (auto& element : batch.Elements)
			{
				SMaterial* material = batch.MaterialSlots[element.MaterialSlotIndex];
				if (material == nullptr)
				{
					continue;
				}

				SRHIShader* mtShader = material->GetShader();
				if (mtShader != _shader)
				{
					continue;
				}

				if (!bCollectBatch)
				{
					relevance = &_drawRelevances[_relevances++];
					relevance->ViewIndex = viewIndex;
					relevance->SceneProxy = sceneProxy;
					relevance->Batch = batch;
					relevance->Elements.clear();
					relevance->Elements.reserve(batch.Elements.size());
					relevance->Materials.clear();
					relevance->Materials.reserve(batch.Elements.size());

					bCollectBatch = true;
				}

				relevance->Elements.emplace_back(element);
				relevance->Materials.emplace_back(material);
			}
		}
	});

	_visibility = view;
}

void SSceneRenderer::RenderScene(SRHIDeviceContext* dc)
{
	dc->SetGraphicsShader(_shader);
	dc->IASetPrimitiveTopology(ERHIPrimitiveTopology::TriangleList);

	RenderWithSceneVisibility(dc);
}

void SSceneRenderer::RenderWithSceneVisibility(SRHIDeviceContext* dc)
{
	for (size_t i = 0; i < _relevances; ++i)
	{
		MeshBatchDrawRelevance& relevance = _drawRelevances[i];
		_visibility->SetupView(dc, _shader, relevance.ViewIndex);

		uint32 vStride = relevance.Batch.VertexFactory->GetVertexStride();
		uint32 iStride = relevance.Batch.VertexFactory->GetIndexStride();

		RHIVertexBufferView vbv;
		vbv.BufferLocation = relevance.Batch.VertexBufferLocation;
		vbv.SizeInBytes = vStride * (uint32)relevance.Batch.VertexBuffer.size();
		vbv.StrideInBytes = vStride;

		RHIIndexBufferView ibv;
		ibv.BufferLocation = relevance.Batch.IndexBufferLocation;
		ibv.SizeInBytes = sizeof(uint32) * (uint32)relevance.Batch.IndexBuffer.size();
		ibv.Format = ERHIVertexElementFormat::R32_UINT;

		dc->IASetVertexBuffers(0, 1, &vbv);
		dc->IASetIndexBuffer(ibv);

		for (size_t j = 0; j < relevance.Elements.size(); ++j)
		{
			MeshBatchElement& element = relevance.Elements[j];
			SMaterial*& material = relevance.Materials[j];

			material->SetupMaterial(dc);
			dc->DrawIndexedInstanced(element.IndexCount, 1, element.StartIndexLocation, element.BaseVertexLocation);
		}
	}
}