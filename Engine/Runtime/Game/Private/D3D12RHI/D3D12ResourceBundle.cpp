// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12ResourceBundle.h"

#include "Engine.h"
#include "D3D12DeviceBundle.h"
#include "D3D12Resource.h"
#include "D3D12ImmediateCommandList.h"
#include "D3D12DeferredCommandList.h"
#include "D3D12Scene.h"
#include "SceneRendering/StaticMesh.h"
#include "RHI/RHICommon.h"
#include "RHI/RHIResourceGC.h"
#include "RHI/RHIResourceGC.h"
#include "PlatformMisc/PlatformImage.h"

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

TRefPtr<IRHIResource> D3D12ResourceBundle::CreateTexture2D(ERHITextureFormat format, PlatformImage* platformImage)
{
	D3D12_RESOURCE_DESC textureDesc = { };
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = platformImage->Width;
	textureDesc.Height = platformImage->Height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = (DXGI_FORMAT)format;
	textureDesc.SampleDesc = { 1, 0 };
	textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_HEAP_PROPERTIES heap = { };
	heap.Type = D3D12_HEAP_TYPE_DEFAULT;

	// Create texture immutable buffer for copy destination.
	ComPtr<ID3D12Resource> resource;
	HR(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource)));

	// Get copyable footprint.
	uint64 totalBytes;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
	device->GetCopyableFootprints(&textureDesc, 0, 1, 0, &layout, nullptr, nullptr, &totalBytes);

	D3D12_RESOURCE_DESC bufferDesc{ };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = totalBytes;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// Create upload heap for copy source that texture pixels.
	ComPtr<ID3D12Resource> uploadHeap;
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	HR(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadHeap)));

	// Copy pixels.
	void* pData;
	HR(uploadHeap->Map(0, nullptr, &pData));
	platformImage->CopyPixels(layout.Footprint.RowPitch, totalBytes, (int8*)pData);
	uploadHeap->Unmap(0, nullptr);

	// Ready to upload to destination texture heap.
	D3D12_RESOURCE_BARRIER barrier = { };
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	D3D12_TEXTURE_COPY_LOCATION dst = { }, src = { };
	dst.pResource = resource.Get();
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;
	src.pResource = uploadHeap.Get();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = layout;

	// Commit copy commands.
	TRefPtr<D3D12DeferredCommandList> deferredCmd = Cast<D3D12DeferredCommandList>(deviceBundle->CreateDeferredCommandList());
	deferredCmd->BeginCommand();
	deferredCmd->CommandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
	deferredCmd->CommandList->ResourceBarrier(1, &barrier);
	deferredCmd->EndCommand();

	RHIResourceGC* resourceGC = deviceBundle->GetResourceGC();
	uint64 issued = resourceGC->IssueNumber();
	resourceGC->AddPendingResource(deferredCmd, issued);
	resourceGC->AddPendingResource(NewObject<D3D12Resource>(uploadHeap.Get()), issued);

	deviceBundle->GetImmediateCommandList()->ExecuteCommandList(deferredCmd.Get());
	resourceGC->SignalNumber(issued);

	return NewObject<D3D12Resource>(resource.Get());
}

TRefPtr<IRHIScene> D3D12ResourceBundle::CreateScene(APlayerController* inPlayerController)
{
	return NewObject<D3D12Scene>(inPlayerController);
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