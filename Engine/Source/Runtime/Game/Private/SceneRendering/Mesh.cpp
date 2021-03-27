// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/Mesh.h"

#include "Engine.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "SceneRendering/Vertex.h"

using namespace std;

Mesh::Mesh(Engine* engine) : Super()
	, engine(engine)
	, deviceBundle(engine->GetDeviceBundle())
{

}

Mesh::~Mesh()
{

}

TComPtr<ID3D12Resource> Mesh::CreateVertexBuffer(span<Vertex const> inVertices)
{
	DirectXCommandQueue* commandQueue = engine->GetPrimaryCommandQueue();
	return deviceBundle->CreateImmutableBuffer(
		commandQueue,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		(const uint8*)inVertices.data(),
		inVertices.size_bytes(),
		D3D12_RESOURCE_FLAG_NONE
	);
}

TComPtr<ID3D12Resource> Mesh::CreateIndexBuffer(span<uint32 const> inVertices)
{
	DirectXCommandQueue* commandQueue = engine->GetPrimaryCommandQueue();
	return deviceBundle->CreateImmutableBuffer(
		commandQueue,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		(const uint8*)inVertices.data(),
		inVertices.size_bytes(),
		D3D12_RESOURCE_FLAG_NONE
	);
}