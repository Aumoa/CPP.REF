// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12ResourceBundle.h"

#include "Engine.h"
#include "D3D12DeviceBundle.h"
#include "D3D12Resource.h"
#include "D3D12ImmediateCommandList.h"
#include "D3D12DeferredCommandList.h"
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
	//deviceBundle = Cast<D3D12DeviceBundle>(GEngine.DeviceBundle);
	//device = deviceBundle->Device;
}

void D3D12ResourceBundle::ReleaseBundle()
{

}

void D3D12ResourceBundle::Commit(IRHICommandList* inCommandList)
{

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