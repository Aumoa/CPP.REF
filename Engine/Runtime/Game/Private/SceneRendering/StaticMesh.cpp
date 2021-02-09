// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/StaticMesh.h"

#include "Engine.h"
#include "RHI/IRHIResource.h"
#include "RHI/RHIVertex.h"
#include "RHI/IRHIDeviceBundle.h"
#include "Materials/Material.h"

using namespace std;

StaticMeshBatch::StaticMeshBatch(const RHIMeshDrawCommand& drawCommand)
{
	this->drawCommand = drawCommand;
}

StaticMeshBatch::~StaticMeshBatch()
{

}

const RHIMeshDrawCommand* StaticMeshBatch::GetDrawCommand() const
{
	return &drawCommand;
}

StaticMesh::StaticMesh() : Super()
{

}

StaticMesh::~StaticMesh()
{

}

MeshBatch* StaticMesh::GetMeshBatch() const
{
	return meshBatch.Get();
}

TRefPtr<StaticMesh> StaticMesh::CreateStaticMesh(span<RHIVertex> vertices, span<uint32> indices, TRefPtr<Material> defaultMaterial)
{
	auto staticMesh = NewObject<StaticMesh>();

	staticMesh->vertexBuffer = GEngine.DeviceBundle->CreateVertexBuffer(vertices);
	staticMesh->indexBuffer = GEngine.DeviceBundle->CreateIndexBuffer(indices);
	staticMesh->material = move(defaultMaterial);

	RHIMeshDrawCommand command;
	command.VertexBufferVirtualAddress = staticMesh->vertexBuffer->GetVirtualAddress();
	command.VertexCount = (uint32)vertices.size();
	command.VertexStride = sizeof(RHIVertex);
	command.IndexBufferVirtualAddress = staticMesh->indexBuffer->GetVirtualAddress();
	command.IndexCount = (uint32)indices.size();
	command.MaterialIndex = staticMesh->material.IsValid ? staticMesh->material->Index : 0;
	staticMesh->meshBatch = NewObject<StaticMeshBatch>(command);

	return staticMesh;
}