// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/StaticMesh.h"

#include "Engine.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXImmediateContext.h"
#include "SceneRendering/Vertex.h"

using namespace std;

void StaticMeshGeometryData::Clear(bool bShrinkToFit)
{
	if (bShrinkToFit)
	{
		VertexBuffer.clear();
		IndexBuffer.clear();
		Subsets.clear();
		Materials.clear();
	}
	else
	{
		vector<Vertex> vertexBufferEmpty;
		VertexBuffer.swap(vertexBufferEmpty);
		vector<uint32> indexBufferEmpty;
		IndexBuffer.swap(indexBufferEmpty);
		vector<StaticMeshSubsetInfo> subsetsEmpty;
		Subsets.swap(subsetsEmpty);
		vector<Material*> materialsEmpty;
		Materials.swap(materialsEmpty);
	}
}

void StaticMeshGeometryData::AddSubset(span<Vertex const> inVertices, span<uint32 const> inIndices, Material* inMaterial)
{
	StaticMeshSubsetInfo subset;
	subset.VertexStart = (uint32)VertexBuffer.size();
	subset.VertexCount = (uint32)inVertices.size();
	subset.IndexStart = (uint32)IndexBuffer.size();
	subset.IndexCount = (uint32)inIndices.size();

	VertexBuffer.insert(VertexBuffer.end(), inVertices.begin(), inVertices.end());
	IndexBuffer.insert(IndexBuffer.end(), inIndices.begin(), inIndices.end());
	Materials.emplace_back(inMaterial);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

StaticMesh::StaticMesh(Engine* engine, const StaticMeshGeometryData& inGeometryData) : Super(engine)
{
	vertexBuffer = CreateVertexBuffer(inGeometryData.VertexBuffer);
	indexBuffer = CreateIndexBuffer(inGeometryData.IndexBuffer);

	DirectXCommandQueue* commandQueue = engine->GetPrimaryCommandQueue();
	DirectXDeviceBundle* deviceBundle = engine->GetDeviceBundle();
	ID3D12Device5* device = deviceBundle->GetDevice();

	uint64 vertexBufferStart = vertexBuffer->GetGPUVirtualAddress();
	uint64 indexBufferStart = indexBuffer->GetGPUVirtualAddress();

	auto GetVertexBufferStart = [&](uint32 inVertexStart) -> D3D12_GPU_VIRTUAL_ADDRESS_AND_STRIDE
	{
		D3D12_GPU_VIRTUAL_ADDRESS_AND_STRIDE r;
		r.StrideInBytes = sizeof(Vertex);
		r.StartAddress = vertexBufferStart + inVertexStart * r.StrideInBytes;
		return r;
	};

	auto GetIndexBufferStart = [&](uint32 inIndexStart) -> D3D12_GPU_VIRTUAL_ADDRESS
	{
		return indexBufferStart + inIndexStart * sizeof(uint32);
	};

	// Write general geometry description.
	vector<D3D12_RAYTRACING_GEOMETRY_DESC> geometries;
	geometries.reserve(inGeometryData.Subsets.size());
	for (auto& subset : inGeometryData.Subsets)
	{
		D3D12_RAYTRACING_GEOMETRY_TRIANGLES_DESC triangle = { };
		triangle.VertexBuffer = GetVertexBufferStart(subset.VertexStart);
		triangle.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		triangle.VertexCount = subset.VertexCount;

		triangle.IndexBuffer = GetIndexBufferStart(subset.IndexStart);
		triangle.IndexFormat = DXGI_FORMAT_R32_UINT;
		triangle.IndexCount = subset.IndexCount;

		D3D12_RAYTRACING_GEOMETRY_DESC geometry = { };
		geometry.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
		geometry.Triangles = triangle;
		geometry.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;

		geometries.emplace_back(geometry);
	}

	// Get description of raytracing scratch and result buffers.
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = { };
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
	inputs.NumDescs = (uint32)geometries.size();
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.pGeometryDescs = geometries.data();

	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuildInfo = { };
	device->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &prebuildInfo);

	// Ready buffers for compute raytracing acceleration structure.
	D3D12_RESOURCE_DESC bufferDesc = { };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = prebuildInfo.ResultDataMaxSizeInBytes;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES heapProp = { };
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	HR(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, nullptr, IID_PPV_ARGS(&accelerationStructure)));
	SetNameAuto(accelerationStructure);

	TComPtr<ID3D12Resource> scratchBuffer;
	bufferDesc.Width = prebuildInfo.ScratchDataSizeInBytes;
	HR(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&scratchBuffer)));
	SetNameAuto(scratchBuffer);

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildDesc = { };
	buildDesc.Inputs = inputs;
	buildDesc.ScratchAccelerationStructureData = scratchBuffer->GetGPUVirtualAddress();
	buildDesc.DestAccelerationStructureData = accelerationStructure->GetGPUVirtualAddress();

	// Build acceleration structure and discard scratch buffer.
	auto DirectXNew(immediateContext, DirectXImmediateContext, deviceBundle, commandQueue, D3D12_COMMAND_LIST_TYPE_DIRECT);
	immediateContext->BeginDraw();
	{
		ID3D12GraphicsCommandList4* commandList = immediateContext->GetCommandList();
		commandList->BuildRaytracingAccelerationStructure(&buildDesc, 0, nullptr);
	}
	immediateContext->EndDraw();

	// Add pending reference for keep scratch buffer until build completed.
	immediateContext->AddPendingReference(scratchBuffer.Get());

	// Compute bounding box.
	boundingBox = ComputeBoundingBox(inGeometryData.VertexBuffer);
}

StaticMesh::~StaticMesh()
{

}

AxisAlignedCube StaticMesh::ComputeBoundingBox(span<Vertex const> vertices)
{
	AxisAlignedCube cube(numeric_limits<float>::max(), numeric_limits<float>::lowest());

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		cube.Min = Vector3::Min(cube.Min, vertices[i].Pos);
		cube.Max = Vector3::Max(cube.Max, vertices[i].Pos);
	}

	return cube;
}