// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12ResourceBundle.h"

#include "Engine.h"
#include "D3D12DeviceBundle.h"
#include "D3D12Resource.h"
#include "D3D12ImmediateCommandList.h"
#include "SceneRendering/StaticMesh.h"
#include "RHI/RHICommon.h"
#include "RHI/RHIResourceGC.h"

using namespace std;

D3D12ResourceBundle::D3D12ResourceBundle() : Super()
	, deviceBundle(nullptr)
	, device(nullptr)
{

}

D3D12ResourceBundle::~D3D12ResourceBundle()
{

}

void D3D12ResourceBundle::InitializeBundle()
{
	deviceBundle = Cast<D3D12DeviceBundle>(GEngine.DeviceBundle);
	device = deviceBundle->Device;
}

void D3D12ResourceBundle::ReleaseBundle()
{

}

void D3D12ResourceBundle::Commit(IRHICommandList* inCommandList)
{

}

TRefPtr<StaticMesh> D3D12ResourceBundle::CreateStaticMesh(const RHIStaticMeshGeometryData& inGeometryData)
{
	TRefPtr<IRHIResource> vertexBuffer = deviceBundle->IRHIDeviceBundle::CreateVertexBuffer(inGeometryData.VertexBuffer);
	TRefPtr<IRHIResource> indexBuffer = deviceBundle->IRHIDeviceBundle::CreateIndexBuffer(inGeometryData.IndexBuffer);

	uint64 vertexBufferStart = vertexBuffer->GetVirtualAddress();
	uint64 indexBufferStart = indexBuffer->GetVirtualAddress();

	auto GetVertexBufferStart = [&](uint32 inVertexStart) -> D3D12_GPU_VIRTUAL_ADDRESS_AND_STRIDE
	{
		D3D12_GPU_VIRTUAL_ADDRESS_AND_STRIDE r;
		r.StrideInBytes = sizeof(RHIVertex);
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
	TRefPtr<IRHIResource> accelerationBuffer = CreateRaytracingAccelerationStructureBuffer(prebuildInfo.ResultDataMaxSizeInBytes);
	TRefPtr<IRHIResource> scratchBuffer = CreateRaytracingScratchBuffer(prebuildInfo.ScratchDataSizeInBytes);

	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildDesc = { };
	buildDesc.Inputs = inputs;
	buildDesc.ScratchAccelerationStructureData = scratchBuffer->GetVirtualAddress();
	buildDesc.DestAccelerationStructureData = accelerationBuffer->GetVirtualAddress();

	// Build acceleration structure and discard scratch buffer.
	TRefPtr<D3D12DeferredCommandList> deferredCmd = Cast<D3D12DeferredCommandList>(deviceBundle->CreateDeferredCommandList());
	deferredCmd->BeginCommand();
	deferredCmd->CommandList->BuildRaytracingAccelerationStructure(&buildDesc, 0, nullptr);
	deferredCmd->EndCommand();

	RHIResourceGC* resourceGC = deviceBundle->GetResourceGC();
	uint64 issued = resourceGC->IssueNumber();
	resourceGC->AddPendingResource(deferredCmd, issued);
	resourceGC->AddPendingResource(scratchBuffer, issued);

	deviceBundle->GetImmediateCommandList()->ExecuteCommandList(deferredCmd.Get());
	resourceGC->SignalNumber(issued);

	return StaticMesh::CreateStaticMesh(inGeometryData, vertexBuffer.Get(), indexBuffer.Get(), accelerationBuffer.Get());
}

TRefPtr<IRHIResource> D3D12ResourceBundle::CreateRaytracingScratchBuffer(uint64 sizeInBytes)
{
	D3D12_RESOURCE_DESC bufferDesc = { };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = (UINT64)sizeInBytes;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES heap = { };
	heap.Type = D3D12_HEAP_TYPE_DEFAULT;

	ComPtr<ID3D12Resource> resource;
	HR(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&resource)));

	return NewObject<D3D12Resource>(resource.Get());
}

TRefPtr<IRHIResource> D3D12ResourceBundle::CreateRaytracingAccelerationStructureBuffer(uint64 sizeInBytes)
{
	D3D12_RESOURCE_DESC bufferDesc = { };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = (UINT64)sizeInBytes;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_HEAP_PROPERTIES heap = { };
	heap.Type = D3D12_HEAP_TYPE_DEFAULT;

	ComPtr<ID3D12Resource> resource;
	HR(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, nullptr, IID_PPV_ARGS(&resource)));

	return NewObject<D3D12Resource>(resource.Get());
}