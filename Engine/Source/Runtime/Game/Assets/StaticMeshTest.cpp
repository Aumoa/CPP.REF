// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "StaticMeshTest.h"
#include "GameStructures.h"
#include "Scene/StaticMeshRenderData.h"
#include "RHI/RHIVertexFactory.h"
#include "RHI/RHIResource.h"

using namespace std;

StaticMeshTest::StaticMeshTest(wstring_view name, RHIVertexFactory* vfactory) : Super(name)
{
	_RenderData = CreateSubobject<StaticMeshRenderData>(vfactory);
}

StaticMeshTest::StaticMeshTest(wstring_view name, RHIVertexFactory* vfactory, span<const RHIVertex> vertices, span<const uint32> indices) : Super(name)
{
	_RenderData = CreateSubobject<StaticMeshRenderData>(nullptr);

	MeshBatch& batch1 = _RenderData->MeshBatches.emplace_back();
	batch1.VertexFactory = vfactory;

	RHIResource* vb = vfactory->CreateVertexBuffer(vertices.data(), vertices.size());
	RHIResource* ib = vfactory->CreateIndexBuffer(indices.data(), indices.size());
	MeshBatchElement& element1 = batch1.Elements.emplace_back() =
	{
		.VertexBufferLocation = vb->GetGPUVirtualAddress(),
		.IndexBufferLocation = ib->GetGPUVirtualAddress(),
		.IndexCount = (uint32)indices.size(),
		.InstanceCount = 1,
		.StartIndexLocation = 0,
		.BaseVertexLocation = 0,
		.StartInstanceLocation = 0,
		.VertexCount = (uint32)vertices.size()
	};

	vb->SetOuter(this);
	ib->SetOuter(this);
}