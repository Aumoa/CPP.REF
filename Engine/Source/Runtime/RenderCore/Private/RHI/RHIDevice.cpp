// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHI/RHIDevice.h"
#include "InternalComPtr.h"
#include "RHI/LogRHI.h"
#include "RHI/RHIDeviceContext.h"
#include "RHI/RHICommandQueue.h"
#include "RHI/RHIDynamicTexture2D.h"
#include "RHI/IRHIImageSourceView.h"

RHIDevice::RHIDevice(bool bDebug) : Super()
	, _bDebug(bDebug)
{
	if (bDebug)
	{
		InitializeDebug();
	}

	InitializeCOM();
	InitializeDXGI();
	InitializeD3D12();
}

RHIDevice::~RHIDevice()
{
}

RHIResource* RHIDevice::CreateImmutableBuffer(ERHIResourceStates initialState, const uint8* buffer, size_t length)
{
	D3D12_RESOURCE_DESC bufferDesc = { };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = (UINT64)length;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_HEAP_PROPERTIES heap = { };
	heap.Type = D3D12_HEAP_TYPE_DEFAULT;

	ComPtr<ID3D12Resource> resource;
	HR(LogRHI, _device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource)));

	ComPtr<ID3D12Resource> uploadHeap;
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	HR(LogRHI, _device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadHeap)));

	void* pData;
	HR(LogRHI, uploadHeap->Map(0, nullptr, &pData));
	memcpy(pData, buffer, length);
	uploadHeap->Unmap(0, nullptr);

	D3D12_RESOURCE_BARRIER barrier = { };
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = (D3D12_RESOURCE_STATES)initialState;

	RHIDeviceContext* cmd = NewObject<RHIDeviceContext>(this, ERHICommandType::Direct);
	cmd->Begin();
	ComPtr<ID3D12GraphicsCommandList> cmdlist;
	HR(LogRHI, cmd->GetCommandList()->QueryInterface(IID_PPV_ARGS(&cmdlist)));
	cmdlist->CopyResource(resource.Get(), uploadHeap.Get());
	cmdlist->ResourceBarrier(1, &barrier);
	cmd->End();

	uint64 signalNumber = _queue->ExecuteDeviceContext(cmd);
	_queue->AddGarbageObject(signalNumber, uploadHeap.Detach());
	_queue->AddGarbageObject(signalNumber, cmd);

	return NewObject<RHIResource>(this, resource.Get());
}

RHIResource* RHIDevice::CreateDynamicBuffer(size_t length)
{
	D3D12_RESOURCE_DESC bufferDesc =
	{
		.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
		.Width = (UINT64)length,
		.Height = 1,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.Format = DXGI_FORMAT_UNKNOWN,
		.SampleDesc = { 1, 0 },
		.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		.Flags = D3D12_RESOURCE_FLAG_NONE
	};

	D3D12_HEAP_PROPERTIES heap = 
	{
		.Type = D3D12_HEAP_TYPE_UPLOAD
	};

	ComPtr<ID3D12Resource> resource;
	HR(LogRHI, _device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource)));

	return NewObject<RHIResource>(this, resource.Get());
}

RHITexture2D* RHIDevice::CreateTexture2D(ERHIResourceStates initialState, ERHIPixelFormat format, uint32 width, uint32 height, std::optional<RHITexture2DClearValue> clearValue, ERHIResourceFlags flags)
{
	D3D12_RESOURCE_DESC bufferDesc =
	{
		.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		.Width = (UINT64)width,
		.Height = height,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.Format = (DXGI_FORMAT)format,
		.SampleDesc = { 1, 0 },
		.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
		.Flags = (D3D12_RESOURCE_FLAGS)flags
	};

	D3D12_HEAP_PROPERTIES heap =
	{
		.Type = D3D12_HEAP_TYPE_DEFAULT
	};

	ComPtr<ID3D12Resource> resource;
	HR(LogRHI, _device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, (D3D12_RESOURCE_STATES)initialState, clearValue.has_value() ? (const D3D12_CLEAR_VALUE*)&clearValue.value() : nullptr, IID_PPV_ARGS(&resource)));

	return NewObject<RHITexture2D>(this, resource.Get());
}

RHIDynamicTexture2D* RHIDevice::CreateDynamicTexture2D(ERHIResourceStates initialState, ERHIPixelFormat format, uint32 width, uint32 height, std::optional<RHITexture2DClearValue> clearValue, ERHIResourceFlags flags)
{
	D3D12_RESOURCE_DESC textureDesc =
	{
		.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		.Width = (UINT64)width,
		.Height = height,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.Format = (DXGI_FORMAT)format,
		.SampleDesc = { 1, 0 },
		.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
		.Flags = (D3D12_RESOURCE_FLAGS)flags
	};

	D3D12_HEAP_PROPERTIES heap =
	{
		.Type = D3D12_HEAP_TYPE_DEFAULT
	};

	ComPtr<ID3D12Resource> resource;
	HR(LogRHI, _device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &textureDesc, (D3D12_RESOURCE_STATES)initialState, clearValue.has_value() ? (const D3D12_CLEAR_VALUE*)&clearValue.value() : nullptr, IID_PPV_ARGS(&resource)));

	// Getting texture copy footprint information.
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	uint64 totalBytes;
	_device->GetCopyableFootprints(&textureDesc, 0, 1, 0, &footprint, nullptr, nullptr, &totalBytes);

	// Resource description for upload buffer.
	D3D12_RESOURCE_DESC bufferDesc =
	{
		.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
		.Width = totalBytes,
		.Height = 1,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.SampleDesc = { 1, 0 },
		.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		.Flags = D3D12_RESOURCE_FLAG_NONE,
	};

	heap = { .Type = D3D12_HEAP_TYPE_UPLOAD };

	// Create upload buffer.
	ComPtr<ID3D12Resource> uploadHeap;
	HR(LogRHI, _device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadHeap)));

	return NewObject<RHIDynamicTexture2D>(this, resource.Get(), uploadHeap.Get(), footprint);
}

RHITexture2D* RHIDevice::CreateTexture2DFromImage(IRHIImageSourceView* imageSource)
{
	// Resource description for texture2D.
	D3D12_RESOURCE_DESC textureDesc =
	{
		.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		.Width = (UINT64)imageSource->GetWidth(),
		.Height = (UINT32)imageSource->GetHeight(),
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.Format = (DXGI_FORMAT)imageSource->GetPixelFormat(),
		.SampleDesc = { 1, 0 },
		.Flags = D3D12_RESOURCE_FLAG_NONE
	};

	D3D12_HEAP_PROPERTIES heap = { .Type = D3D12_HEAP_TYPE_DEFAULT };

	// Create texture2D resource.
	ComPtr<ID3D12Resource> resource;
	HR(LogRHI, _device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource)));

	// Getting texture copy footprint information.
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
	uint64 totalBytes;
	_device->GetCopyableFootprints(&textureDesc, 0, 1, 0, &footprint, nullptr, nullptr, &totalBytes);

	// Resource description for upload buffer.
	D3D12_RESOURCE_DESC bufferDesc =
	{
		.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
		.Width = totalBytes,
		.Height = 1,
		.DepthOrArraySize = 1,
		.MipLevels = 1,
		.SampleDesc = { 1, 0 },
		.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		.Flags = D3D12_RESOURCE_FLAG_NONE,
	};

	heap = { .Type = D3D12_HEAP_TYPE_UPLOAD };

	// Create upload buffer.
	ComPtr<ID3D12Resource> uploadHeap;
	HR(LogRHI, _device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadHeap)));

	void* pData;
	HR(LogRHI, uploadHeap->Map(0, nullptr, &pData));
	imageSource->CopyPixels((uint32)footprint.Footprint.RowPitch, (uint32)totalBytes, pData);
	uploadHeap->Unmap(0, nullptr);

	// Make resource barrier state.
	D3D12_RESOURCE_BARRIER barrier = { };
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	// Create device context and flush commands.
	RHIDeviceContext* cmd = NewObject<RHIDeviceContext>(this, ERHICommandType::Direct);

	D3D12_TEXTURE_COPY_LOCATION locationSrc = {};
	locationSrc.pResource = uploadHeap.Get();
	locationSrc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	locationSrc.PlacedFootprint = footprint;

	D3D12_TEXTURE_COPY_LOCATION locationDst = {};
	locationDst.pResource = resource.Get();
	locationDst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	locationDst.SubresourceIndex = 0;

	cmd->Begin();
	auto* cmdlist = static_cast<ID3D12GraphicsCommandList*>(cmd->GetCommandList());
	cmdlist->CopyTextureRegion(&locationDst, 0, 0, 0, &locationSrc, nullptr);
	cmdlist->ResourceBarrier(1, &barrier);
	cmd->End();

	// Execute commands.
	uint64 signalNumber = _queue->ExecuteDeviceContext(cmd);
	_queue->AddGarbageObject(signalNumber, uploadHeap.Detach());
	_queue->AddGarbageObject(signalNumber, cmd);

	return NewObject<RHITexture2D>(this, resource.Get());
}

void RHIDevice::InitializeDebug()
{
	SE_LOG(LogRHI, Info, L"----- Initialize Direct3D 12 debug layer.");

	if (ComPtr<ID3D12Debug> debug; SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
	}
	else
	{
		SE_LOG(LogRHI, Warning, L"Direct3D 12 debug layer could not be enabled.");
	}

	SE_LOG(LogRHI, Info, L"----- Done!");
}

void RHIDevice::InitializeCOM()
{
	SE_LOG(LogRHI, Info, L"----- Initialize COM subsystem.");

	HR(LogRHI, CoInitializeEx(nullptr, COINIT::COINIT_MULTITHREADED));
	SE_LOG(LogRHI, Info, L"COM initialized.");

	SE_LOG(LogRHI, Info, L"----- Done!");
}

void RHIDevice::InitializeDXGI()
{
	SE_LOG(LogRHI, Info, L"----- Initialize DXGI subsystem.");

	uint32 flags = _bDebug ? DXGI_CREATE_FACTORY_DEBUG : 0;
	HR(LogRHI, CreateDXGIFactory2(flags, IID_PPV_ARGS(&_factory)));
	SE_LOG(LogRHI, Info, L"DXGI factory created.");

	SE_LOG(LogRHI, Info, L"----- Done!");
}

void RHIDevice::InitializeD3D12()
{
	constexpr double InvGiB = 1.0 / 1024.0 / 1024.0 / 1024.0;

	SE_LOG(LogRHI, Info, L"----- Initialize D3D12 subsystem.");

	SE_LOG(LogRHI, Info, L"Finding most suitable physical device.");
	ComPtr<IDXGIAdapter1> adapter;
	for (int32 i = 0; SUCCEEDED(_factory->EnumAdapters1((uint32)i, &adapter)); ++i)
	{
		DXGI_ADAPTER_DESC1 desc;
		if (FAILED(adapter->GetDesc1(&desc)))
		{
			adapter.Reset();
			continue;
		}

		if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0)
		{
			adapter.Reset();
			continue;
		}

		SE_LOG(LogRHI, Info, L"Suitable adapter found. Adapter: {}, VideoMemory: {:.2} GiB", desc.Description, desc.DedicatedVideoMemory * InvGiB);
		break;
	}

	if (!adapter.IsSet())
	{
		SE_LOG(LogRHI, Fatal, L"Could not find suitable physical device.");
		return;
	}

	HR(LogRHI, D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device)));
	_queue = NewObject<RHICommandQueue>(this, ERHICommandType::Direct);
	SE_LOG(LogRHI, Info, L"Direct3D 12 device created with feature level 11_0.");

	SE_LOG(LogRHI, Info, L"----- Done!");
}