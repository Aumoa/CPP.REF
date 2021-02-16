// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/StaticMesh.h"

#include "Engine.h"
#include "RHI/IRHIResource.h"
#include "RHI/RHIVertex.h"
#include "RHI/IRHIDeviceBundle.h"

using namespace std;

StaticMeshBatch::StaticMeshBatch()
{

}

StaticMeshBatch::~StaticMeshBatch()
{

}

void StaticMeshBatch::SetMeshDrawCommand(const RHIMeshDrawCommand& inDrawCommand)
{
	drawCommand = inDrawCommand;
}

const RHIMeshDrawCommand* StaticMeshBatch::GetDrawCommand() const
{
	return &drawCommand;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	return CreateStaticMesh(vertices, indices, defaultMaterial, ComputeBoundingBox(vertices));
}

TRefPtr<StaticMesh> StaticMesh::CreateStaticMesh(span<RHIVertex> vertices, span<uint32> indices, TRefPtr<Material> defaultMaterial, const AxisAlignedCube& inBoundingBox)
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

	auto batch = NewObject<StaticMeshBatch>();
	batch->SetMeshDrawCommand(command);

	staticMesh->meshBatch = batch;
	staticMesh->boundingBox = inBoundingBox;

	return staticMesh;
}

AxisAlignedCube StaticMesh::ComputeBoundingBox(span<RHIVertex> vertices)
{
	AxisAlignedCube cube(Vector3::Zero, Vector3::Zero);

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		cube.Min = Vector3::Min(cube.Min, vertices[i].Pos);
		cube.Max = Vector3::Max(cube.Max, vertices[i].Pos);
	}

	return cube;
}