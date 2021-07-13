// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "RHIDevice.h"
#include "LogRHI.h"
#include "RHIDeviceContext.h"
#include "RHICommandQueue.h"
#include "RHITexture2D.h"

using namespace std;

using enum ELogVerbosity;

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

	RHIDeviceContext* cmd = CreateSubobject<RHIDeviceContext>(this, ERHICommandType::Direct);
	cmd->Begin();
	ComPtr<ID3D12GraphicsCommandList> cmdlist;
	HR(LogRHI, cmd->GetCommandList()->QueryInterface(IID_PPV_ARGS(&cmdlist)));
	cmdlist->CopyResource(resource.Get(), uploadHeap.Get());
	cmdlist->ResourceBarrier(1, &barrier);
	cmd->End();

	uint64 signalNumber = _queue->ExecuteDeviceContext(cmd);
	_queue->AddGarbageObject(signalNumber, uploadHeap.Detach());
	_queue->AddGarbageObject(signalNumber, cmd);

	return CreateSubobject<RHIResource>(this, resource.Get());
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

	return CreateSubobject<RHIResource>(this, resource.Get());
}

RHITexture2D* RHIDevice::CreateTexture2D(ERHIResourceStates initialState, ERHIPixelFormat format, uint32 width, uint32 height, optional<RHITexture2DClearValue> clearValue, ERHIResourceFlags flags)
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

	return CreateSubobject<RHITexture2D>(this, resource.Get());
}

void RHIDevice::InitializeDebug()
{
	LogSystem::Log(LogRHI, Info, L"----- Initialize Direct3D 12 debug layer.");

	if (ComPtr<ID3D12Debug> debug; SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
	}
	else
	{
		LogSystem::Log(LogRHI, Warning, L"Direct3D 12 debug layer could not be enabled.");
	}

	LogSystem::Log(LogRHI, Info, L"----- Done!");
}

void RHIDevice::InitializeCOM()
{
	LogSystem::Log(LogRHI, Info, L"----- Initialize COM subsystem.");

	HR(LogRHI, CoInitializeEx(nullptr, COINIT::COINIT_MULTITHREADED));
	LogSystem::Log(LogRHI, Info, L"COM initialized.");

	LogSystem::Log(LogRHI, Info, L"----- Done!");
}

void RHIDevice::InitializeDXGI()
{
	LogSystem::Log(LogRHI, Info, L"----- Initialize DXGI subsystem.");

	uint32 flags = _bDebug ? DXGI_CREATE_FACTORY_DEBUG : 0;
	HR(LogRHI, CreateDXGIFactory2(flags, IID_PPV_ARGS(&_factory)));
	LogSystem::Log(LogRHI, Info, L"DXGI factory created.");

	LogSystem::Log(LogRHI, Info, L"----- Done!");
}

void RHIDevice::InitializeD3D12()
{
	constexpr double InvGiB = 1.0 / 1024.0 / 1024.0 / 1024.0;

	LogSystem::Log(LogRHI, Info, L"----- Initialize D3D12 subsystem.");

	LogSystem::Log(LogRHI, Info, L"Finding most suitable physical device.");
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

		LogSystem::Log(LogRHI, Info, L"Suitable adapter found. Adapter: {}, VideoMemory: {:.2} GiB", desc.Description, desc.DedicatedVideoMemory * InvGiB);
		break;
	}

	if (!adapter.IsSet())
	{
		LogSystem::Log(LogRHI, Fatal, L"Could not find suitable physical device.");
		return;
	}

	HR(LogRHI, D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device)));
	_queue = CreateSubobject<RHICommandQueue>(this, ERHICommandType::Direct);
	LogSystem::Log(LogRHI, Info, L"Direct3D 12 device created with feature level 11_0.");

	LogSystem::Log(LogRHI, Info, L"----- Done!");
}