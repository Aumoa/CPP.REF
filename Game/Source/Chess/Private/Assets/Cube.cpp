// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Assets/Cube.h"
#include "GameEngine.h"
#include "Scene/StaticMeshRenderData.h"
#include "Shaders/ColorShader/ColorVertexFactory.h"
#include "Misc/GeometryGenerator.h"
#include "RHI/IRHIBuffer.h"

using namespace std;

Cube::Cube(wstring_view name, SGameEngine* engine) : Super(L"", name, GetRenderData(engine))
{
}

SStaticMeshRenderData* Cube::GetRenderData(SGameEngine* engine)
{
	//static SStaticMeshRenderData* renderData = nullptr;
	//if (renderData == nullptr)
	//{
	//	renderData = engine->NewObject<SStaticMeshRenderData>();
	//	MeshBatch & batch = renderData->MeshBatches.emplace_back();
	//	batch.VertexFactory = engine->GetColorVertexFactory();
	//	batch.MaterialSlots.emplace_back() = nullptr;
	//	GeometryGenerator::ComputeBox(batch.VertexBuffer, batch.IndexBuffer, 1.0f, false, false);

	//	batch.Elements.emplace_back() =
	//	{
	//		.IndexCount = (uint32)batch.IndexBuffer.size(),
	//		.InstanceCount = 1,
	//		.StartIndexLocation = 0,
	//		.BaseVertexLocation = 0,
	//		.VertexCount = (uint32)batch.VertexBuffer.size(),
	//		.MaterialSlotIndex = 0
	//	};

	//	SRHIResource* vb = batch.VertexFactory->CreateVertexBuffer(batch.VertexBuffer.data(), batch.VertexBuffer.size());
	//	SRHIResource* ib = batch.VertexFactory->CreateIndexBuffer(batch.IndexBuffer.data(), batch.IndexBuffer.size());
	//	vb->SetOuter(renderData);
	//	ib->SetOuter(renderData);
	//	batch.VertexBufferLocation = vb->GetGPUVirtualAddress();
	//	batch.IndexBufferLocation = ib->GetGPUVirtualAddress();
	//}

	//return renderData;
	return nullptr;
}