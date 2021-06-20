// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "StaticMeshRenderData.h"
#include "GameStructures.h"
#include "RHI/RHIVertexFactory.h"
#include "RHI/RHIResource.h"

StaticMeshRenderData::StaticMeshRenderData(RHIVertexFactory* vfactory) : Super()
{
	if (vfactory != nullptr)
	{
		// TEST IMPLEMENTATION
		MeshBatch& batch1 = MeshBatches.emplace_back();
		batch1.VertexFactory = vfactory;

		RHIVertex triangle[3] =
		{
			{.Position = Vector3(0.0f, +1.0f, 0.0f), .Color = NamedColors::Red },
			{.Position = Vector3(1.0f, -1.0f, 0.0f), .Color = NamedColors::Green },
			{.Position = Vector3(-1.0f, -1.0f, 0.0f), .Color = NamedColors::Blue },
		};

		uint32 ids[3] = { 0, 1, 2 };

		RHIResource* vb = vfactory->CreateVertexBuffer(triangle, 3);
		RHIResource* ib = vfactory->CreateIndexBuffer(ids, 3);
		MeshBatchElement& element1 = batch1.Elements.emplace_back() =
		{
			.VertexBufferLocation = vb->GetGPUVirtualAddress(),
			.IndexBufferLocation = ib->GetGPUVirtualAddress(),
			.IndexCount = 3,
			.InstanceCount = 1,
			.StartIndexLocation = 0,
			.BaseVertexLocation = 0,
			.StartInstanceLocation = 0,
			.VertexCount = 3
		};

		vb->SetOuter(this);
		ib->SetOuter(this);
	}
}