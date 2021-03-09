// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXDeviceBundle.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXCommandQueue.h"
#include "DirectX/DirectXImmediateContext.h"
#include "Logging/LogMacros.h"
#include "COM/COMMinimal.h"

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

}

IDXGIFactory2* DirectXDeviceBundle::GetFactory() const
{
	return dxgiFactory.Get();
}

ID3D12Device5* DirectXDeviceBundle::GetDevice() const
{
	return device.Get();
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

	auto DirectXNew(immediateContext, DirectXImmediateContext, this, commandQueue, D3D12_COMMAND_LIST_TYPE_DIRECT);
	ID3D12GraphicsCommandList4* commandList = immediateContext->GetCommandList();

	// Copy resource from CPU storage(UploadHeap) to GPU storage(DefaultHeap).
	immediateContext->BeginDraw();
	{
		commandList->CopyResource(resource.Get(), uploadHeap.Get());
		commandList->ResourceBarrier(1, &barrier);
	}
	immediateContext->EndDraw();

	// Add pending reference for keep upload heap until copy operation will completed.
	immediateContext->AddPendingReference(uploadHeap.Get());

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