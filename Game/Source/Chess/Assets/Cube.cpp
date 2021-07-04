// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Cube.h"
#include "GameEngine.h"
#include "Scene/StaticMeshRenderData.h"
#include "Shaders/ColorShader/ColorVertexFactory.h"
#include "Misc/GeometryGenerator.h"
#include "RHI/RHIResource.h"

using namespace std;

Cube::Cube(wstring_view name, GameEngine* engine) : Super(name, GetRenderData(engine))
{
}

StaticMeshRenderData* Cube::GetRenderData(GameEngine* engine)
{
	static StaticMeshRenderData* renderData = nullptr;
	if (renderData == nullptr)
	{
		renderData = engine->CreateSubobject<StaticMeshRenderData>();
		MeshBatch & batch = renderData->MeshBatches.emplace_back();
		batch.VertexFactory = engine->GetColorVertexFactory();
		batch.MaterialSlots.emplace_back() = nullptr;
		GeometryGenerator::ComputeBox(batch.VertexBuffer, batch.IndexBuffer, 1.0f, false, false);

		batch.Elements.emplace_back() =
		{
			.IndexCount = (uint32)batch.IndexBuffer.size(),
			.InstanceCount = 1,
			.StartIndexLocation = 0,
			.BaseVertexLocation = 0,
			.VertexCount = (uint32)batch.VertexBuffer.size(),
			.MaterialSlotIndex = 0
		};

		RHIResource* vb = batch.VertexFactory->CreateVertexBuffer(batch.VertexBuffer.data(), batch.VertexBuffer.size());
		RHIResource* ib = batch.VertexFactory->CreateIndexBuffer(batch.IndexBuffer.data(), batch.IndexBuffer.size());
		vb->SetOuter(renderData);
		ib->SetOuter(renderData);
		batch.VertexBufferLocation = vb->GetGPUVirtualAddress();
		batch.IndexBufferLocation = ib->GetGPUVirtualAddress();
	}

	return renderData;
}