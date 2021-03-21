// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXDeviceBundle.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXCommandQueue.h"
#include "DirectX/DirectXDeviceContext.h"
#include "DirectX/DirectXDynamicBufferAllocator.h"
#include "Logging/LogMacros.h"
#include "COM/COMMinimal.h"
#include "COM/COMImage.h"

using namespace std;

#define WITH_REPORT 0

DirectXDeviceBundle::DirectXDeviceBundle() : Super()
{
	UINT flag = 0;

#if defined(_DEBUG) && !defined(_DEBUGGAME)
	if (TComPtr<ID3D12Debug> debug; SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
		SE_LOG(LogDirectX, Verbose, "D3D12 debug layer enabled.");
	}
	else
	{
		SE_LOG(LogDirectX, Warning, "Cannot enable D3D12 debug layer.");
	}
	flag |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	// Initialize DirectX Graphics Infrastructure Factory.
	HR(CreateDXGIFactory2(flag, IID_PPV_ARGS(&dxgiFactory)));

	TComPtr<IDXGIAdapter1> adapter;
	for (int32 i = 0; SUCCEEDED(dxgiFactory->EnumAdapters1((UINT)i, &adapter)); ++i)
	{
		if (!IsAdapterSuitable(adapter.Get()))
		{
			continue;
		}

		if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device))))
		{
			continue;
		}

		if (!IsDeviceSuitable(device.Get()))
		{
			continue;
		}

		DXGI_ADAPTER_DESC1 desc = { };
		HR(adapter->GetDesc1(&desc));

		SE_LOG(LogDirectX, Verbose, L"That supported feature level 12_1 device named to {0} is selected.", desc.Description);
		break;
	}

	if (!device.IsValid)
	{
		SE_LOG(LogDirectX, Error, "Failed to detect adapter that support feature level 12_1. Create device with software platform.");
		throw COMException(E_NOINTERFACE);
	}
}

DirectXDeviceBundle::~DirectXDeviceBundle()
{
#if WITH_REPORT && defined(_DEBUG)
	if (TComPtr<ID3D12DebugDevice> debugDevice; device.IsValid && SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&debugDevice))))
	{
		// Report live debug objects.
		HRESULT hr = debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
		if (FAILED(hr))
		{
			SE_LOG(LogDirectX, Warning, L"Failed to report live device objects.");
			return;
		}
	}
#endif
}

IDXGIFactory2* DirectXDeviceBundle::GetFactory() const
{
	return dxgiFactory.Get();
}

ID3D12Device5* DirectXDeviceBundle::GetDevice() const
{
	return device.Get();
}

DirectXDynamicBufferAllocator* DirectXDeviceBundle::GetOrCreateDynamicBufferAllocator(uint64 allocateUnit)
{
	if (allocateUnit == 0)
	{
		SE_LOG(LogDirectX, Error, L"allocateUnit is zero.");
		return nullptr;
	}

	// Size will be aligned by 256 bytes.
	allocateUnit = (allocateUnit + 255) & ~255;

	if (auto it = dynamicBufferAllocators.find(allocateUnit); it != dynamicBufferAllocators.end())
	{
		return it->second.Get();
	}
	else
	{
		return dynamicBufferAllocators.emplace(allocateUnit, NewObject<DirectXDynamicBufferAllocator>(this, allocateUnit, true)).first->second.Get();
	}
}

DirectXDynamicBufferAllocator* DirectXDeviceBundle::GetDynamicBufferAllocator(uint64 allocateUnit) const
{
	// Size will be aligned by 256 bytes.
	allocateUnit = (allocateUnit + 255) & ~255;

	if (auto it = dynamicBufferAllocators.find(allocateUnit); it != dynamicBufferAllocators.end())
	{
		return it->second.Get();
	}
	else
	{
		SE_LOG(LogDirectX, Error, L"There is not contains buffer allocator for allocate unit: {0}. See GetOrCreateDynamicBufferAllocator function.", allocateUnit);
		return nullptr;
	}
}

TComPtr<ID3D12Resource> DirectXDeviceBundle::CreateImmutableBuffer(DirectXCommandQueue* commandQueue, D3D12_RESOURCE_STATES initialState, const uint8* initialBuffer, size_t sizeInBytes, D3D12_RESOURCE_FLAGS flags)
{
	//
	// Create committed default buffer.
	D3D12_RESOURCE_DESC bufferDesc = { };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = (UINT64)sizeInBytes;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = flags;

	D3D12_HEAP_PROPERTIES heap = { };
	heap.Type = D3D12_HEAP_TYPE_DEFAULT;

	TComPtr<ID3D12Resource> resource;
	HR(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource)));

	// Create committed upload buffer.
	TComPtr<ID3D12Resource> uploadHeap;
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	HR(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadHeap)));

	// Copy upload data to upload buffer.
	void* pData;
	HR(uploadHeap->Map(0, nullptr, &pData));
	memcpy(pData, initialBuffer, sizeInBytes);
	uploadHeap->Unmap(0, nullptr);

	D3D12_RESOURCE_BARRIER barrier = { };
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = initialState;

	auto DirectXNew(deviceContext, DirectXDeviceContext, this, D3D12_COMMAND_LIST_TYPE_DIRECT);

	// Copy resource from CPU storage(UploadHeap) to GPU storage(DefaultHeap).
	deviceContext->BeginDraw();
	{
		ID3D12GraphicsCommandList4* commandList = deviceContext->GetCommandList();
		commandList->CopyResource(resource.Get(), uploadHeap.Get());
		commandList->ResourceBarrier(1, &barrier);
	}
	deviceContext->EndDraw();

	// Add pending reference for keep upload heap until copy operation will completed.
	deviceContext->AddPendingReference(uploadHeap.Get());

	// Execute commands and enqueue pending reference.
	commandQueue->ExecuteCommandLists(deviceContext.Get());
	commandQueue->AddPendingReference(move(deviceContext));

	return resource;
}

TComPtr<ID3D12Resource> DirectXDeviceBundle::CreateDynamicBuffer(size_t sizeInBytes)
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

	D3D12_HEAP_PROPERTIES heap = { };
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;

	TComPtr<ID3D12Resource> resource;
	HR(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource)));

	return resource;
}

TComPtr<ID3D12Resource> DirectXDeviceBundle::CreateTexture2D(COMImage* image, DirectXCommandQueue* commandQueue, DXGI_FORMAT format, D3D12_RESOURCE_STATES initialState)
{
	D3D12_RESOURCE_DESC textureDesc = { };
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = image->Width;
	textureDesc.Height = image->Height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = (DXGI_FORMAT)format;
	textureDesc.SampleDesc = { 1, 0 };
	textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_HEAP_PROPERTIES heap = { };
	heap.Type = D3D12_HEAP_TYPE_DEFAULT;

	// Create texture immutable buffer for copy destination.
	TComPtr<ID3D12Resource> resource;
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
	TComPtr<ID3D12Resource> uploadHeap;
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	HR(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadHeap)));

	// Copy pixels.
	void* pData;
	HR(uploadHeap->Map(0, nullptr, &pData));
	image->CopyPixels(layout.Footprint.RowPitch, totalBytes, (int8*)pData);
	uploadHeap->Unmap(0, nullptr);

	// Ready to upload to destination texture heap.
	D3D12_RESOURCE_BARRIER barrier = { };
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = initialState;

	D3D12_TEXTURE_COPY_LOCATION dst = { }, src = { };
	dst.pResource = resource.Get();
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;
	src.pResource = uploadHeap.Get();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = layout;

	// Commit copy commands.
	auto DirectXNew(deviceContext, DirectXDeviceContext, this, D3D12_COMMAND_LIST_TYPE_DIRECT);

	// Copy resource from CPU storage(UploadHeap) to GPU storage(DefaultHeap).
	deviceContext->BeginDraw();
	{
		ID3D12GraphicsCommandList4* commandList = deviceContext->GetCommandList();
		commandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
		commandList->ResourceBarrier(1, &barrier);
	}
	deviceContext->EndDraw();

	// Add pending reference for keep upload heap until copy operation will completed.
	deviceContext->AddPendingReference(uploadHeap.Get());

	// Execute commands and enqueue pending reference.
	commandQueue->ExecuteCommandLists(deviceContext.Get());
	commandQueue->AddPendingReference(move(deviceContext));

	return resource;
}

bool DirectXDeviceBundle::IsAdapterSuitable(IDXGIAdapter1* adapter) const
{
	DXGI_ADAPTER_DESC1 desc = { };
	HR(adapter->GetDesc1(&desc));

	if (desc.Flags == DXGI_ADAPTER_FLAG_REMOTE || desc.Flags == DXGI_ADAPTER_FLAG_SOFTWARE) {
		// Is remote or software implement.
		return false;
	}

	return true;
}

bool DirectXDeviceBundle::IsDeviceSuitable(ID3D12Device5* device) const
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options = { };
	HR(device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options, sizeof(options)));

	// Must support raytracing tier 1.0.
	if (options.RaytracingTier >= D3D12_RAYTRACING_TIER_1_0)
	{
		return true;
	}

	return false;
}