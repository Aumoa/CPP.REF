// Copyright 2020 Aumoa.lib. All right reserved.

#include "SceneRendering/StaticMesh.h"

#include "Engine.h"
#include "RHI/IRHIResource.h"
#include "RHI/RHIVertex.h"
#include "RHI/IRHIDeviceBundle.h"

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
	RHIVertex vertices[3] =
	{
		{ Vector3(0, 1, 0), Color::Red },
		{ Vector3(1, -1, 0), Color::Green },
		{ Vector3(-1, -1, 0), Color::Blue }
	};

	uint32 indices[3] =
	{
		0, 1, 2
	};

	vertexBuffer = GEngine.DeviceBundle->CreateVertexBuffer(vertices);
	indexBuffer = GEngine.DeviceBundle->CreateIndexBuffer(indices);

	RHIMeshDrawCommand command;

	command.VertexBufferVirtualAddress = vertexBuffer->GetVirtualAddress();
	command.VertexCount = 3;

	command.VertexStride = sizeof(RHIVertex);
	command.IndexBufferVirtualAddress = indexBuffer->GetVirtualAddress();
	command.IndexCount = 3;

	meshBatch = NewObject<StaticMeshBatch>(command);
}

StaticMesh::~StaticMesh()
{

}

MeshBatch* StaticMesh::GetMeshBatch() const
{
	return meshBatch.Get();
}