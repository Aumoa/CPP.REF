// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12DeviceBundle.h"

#include "Application.h"
#include "Logging/LogMacros.h"
#include "D3D12SwapChain.h"
#include "D3D12ImmediateCommandList.h"
#include "D3D12CommandFence.h"
#include "D3D12RenderTargetView.h"
#include "D3D12OfflineDescriptorManager.h"
#include "D3D12OfflineDescriptorIndex.h"
#include "D3D12Resource.h"
#include "D3D12DeferredCommandList.h"
#include "D3D12Fence.h"
#include "D3D12DepthStencilView.h"
#include "D3D12ShaderResourceView.h"
#include "D3D12DynamicBufferManager.h"
#include "D3D12DynamicBuffer.h"
#include "D3D12RenderTarget.h"
#include "D3D12OnlineDescriptorPatch.h"
#include "D3D12Shader.h"
#include "D3D12SBTAllocator.h"
#include "RHI/RHIVertex.h"
#include "RHI/RHIResourceGC.h"
#include "RHI/RHITextureClearValue.h"
#include "RHI/RHIShaderLibrary.h"
#include "PlatformMisc/PlatformImage.h"

using namespace std;

D3D12DeviceBundle* D3D12DeviceBundle::instance = nullptr;

D3D12DeviceBundle::D3D12DeviceBundle() : Super()
{
#if defined(_DEBUG) && !defined(_DEBUGGAME)
	if (ComPtr<ID3D12Debug> debug; SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
		SE_LOG(LogD3D12RHI, Verbose, "D3D12 debug layer enabled.");
	}
	else
	{
		SE_LOG(LogD3D12RHI, Warning, "Cannot enable D3D12 debug layer.");
	}
#endif
}

D3D12DeviceBundle::~D3D12DeviceBundle()
{
	CoUninitialize();
}

void D3D12DeviceBundle::InitializeBundle()
{
	if (instance != nullptr)
	{
		SE_LOG(LogD3D12RHI, Error, "Bundle duplicate detected.");
		throw HResultException(E_FAIL);
	}

	SE_LOG(LogD3D12RHI, Verbose, "Initialize D3D12 device bundle.");

	InitializeCOM();
	InitializeDXGI();
	InitializeD3D12();
	InitializeShaders();

	resourceGC = NewObject<RHIResourceGC>();

	instance = this;

	GApplication.Sizing += bind_delegate(Application_OnSizing);
}

void D3D12DeviceBundle::ReleaseBundle()
{
	instance = nullptr;
}

void D3D12DeviceBundle::Commit(IRHICommandList* inCommandList)
{

}

IRHISwapChain* D3D12DeviceBundle::GetSwapChain() const
{
	return swapChain.Get();
}

IRHIImmediateCommandList* D3D12DeviceBundle::GetImmediateCommandList() const
{
	return immediateCommandList.Get();
}

RHIShaderLibrary* D3D12DeviceBundle::GetShaderLibrary() const
{
	return shaderLibrary.Get();
}

RHIResourceGC* D3D12DeviceBundle::GetResourceGC() const
{
	return resourceGC.Get();
}

TRefPtr<IRHICommandFence> D3D12DeviceBundle::CreateCommandFence()
{
	return NewObject<D3D12CommandFence>(d3d12Device.Get());
}

TRefPtr<IRHIRenderTargetView> D3D12DeviceBundle::CreateRenderTargetView(IRHIResource* resource)
{
	ID3D12Resource* resource1 = Cast<D3D12Resource>(resource)->Resource;
	D3D12OfflineDescriptorIndex index = rtvManager->Alloc();
	d3d12Device->CreateRenderTargetView(resource1, nullptr, index.Handle);
	return NewObject<D3D12RenderTargetView>(resource1, index);
}

TRefPtr<IRHIDepthStencilView> D3D12DeviceBundle::CreateDepthStencilView(IRHIResource* resource, ERHITextureFormat inViewFormat)
{
	if (inViewFormat == ERHITextureFormat::Unknown)
	{
		inViewFormat = resource->GetDesc().Format;
	}

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{ };
	dsvDesc.Format = (DXGI_FORMAT)inViewFormat;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	ID3D12Resource* resource1 = Cast<D3D12Resource>(resource)->Resource;
	D3D12OfflineDescriptorIndex index = dsvManager->Alloc();
	d3d12Device->CreateDepthStencilView(resource1, &dsvDesc, index.Handle);
	return NewObject<D3D12DepthStencilView>(resource1, index);
}

TRefPtr<IRHIShaderResourceView> D3D12DeviceBundle::CreateTextureView(IRHIResource* resource, ERHITextureFormat inViewFormat)
{
	if (inViewFormat == ERHITextureFormat::Unknown)
	{
		inViewFormat = resource->GetDesc().Format;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{ };
	srvDesc.Format = (DXGI_FORMAT)inViewFormat;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;

	ID3D12Resource* resource1 = Cast<D3D12Resource>(resource)->Resource;
	D3D12OfflineDescriptorIndex index = srvManager->Alloc();
	d3d12Device->CreateShaderResourceView(resource1, &srvDesc, index.Handle);
	return NewObject<D3D12SingleShaderResourceViewNode>(resource1, index);
}

TRefPtr<IRHIShaderResourceView> D3D12DeviceBundle::CreateTextureGroupView(IRHIResource* const* inResources, size_t count)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC nullSRV = { };
	nullSRV.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	nullSRV.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	nullSRV.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	auto multiSRV = NewObject<D3D12IndependentShaderResourceView>(d3d12Device.Get(), count);

	for (size_t i = 0; i < count; ++i)
	{
		auto d3d12Resource = Cast<D3D12Resource>(inResources[i]);
		ID3D12Resource* resource = nullptr;
		if (d3d12Resource != nullptr)
		{
			resource = d3d12Resource->Resource;
		}

		multiSRV->CreateView(i, resource, resource == nullptr ? &nullSRV : nullptr);
	}

	return multiSRV;
}

TRefPtr<IRHIDeferredCommandList> D3D12DeviceBundle::CreateDeferredCommandList()
{
	return NewObject<D3D12DeferredCommandList>(d3d12Device.Get());
}

TRefPtr<IRHIFence> D3D12DeviceBundle::CreateFence()
{
	return NewObject<D3D12Fence>();
}

TRefPtr<IRHIOnlineDescriptorPatch> D3D12DeviceBundle::CreateOnlineDescriptorPatch()
{
	return NewObject<D3D12OnlineDescriptorPatch>();
}

TRefPtr<IRHIShaderBindingTable> D3D12DeviceBundle::CreateShaderBindingTable(IRHIShader* inShader)
{
	return NewObject<D3D12SBTAllocator>(d3d12Device.Get(), Cast<D3D12Shader>(inShader));
}

TRefPtr<IRHIResource> D3D12DeviceBundle::CreateVertexBuffer(const RHIVertex* vertices, size_t count)
{
	ComPtr<ID3D12Resource> resource = CreateImmutableBuffer(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, (const uint8*)vertices, count * sizeof(RHIVertex));
	return NewObject<D3D12Resource>(resource.Get());
}

TRefPtr<IRHIResource> D3D12DeviceBundle::CreateIndexBuffer(const uint32* indices, size_t count)
{
	ComPtr<ID3D12Resource> resource = CreateImmutableBuffer(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, (const uint8*)indices, count * sizeof(uint32));
	return NewObject<D3D12Resource>(resource.Get());
}

#define ALIGN_256(x) ((x + 255) & ~255)

TRefPtr<IRHIResource> D3D12DeviceBundle::CreateDynamicBuffer(size_t sizeInBytes)
{
	size_t align_size = ALIGN_256(sizeInBytes);

	if (align_size > 2048)
	{
		D3D12_RESOURCE_DESC bufferDesc = { };
		bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		bufferDesc.Width = (uint64)sizeInBytes;
		bufferDesc.Height = 1;
		bufferDesc.DepthOrArraySize = 1;
		bufferDesc.MipLevels = 1;
		bufferDesc.SampleDesc = { 1, 0 };
		bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		D3D12_HEAP_PROPERTIES heapProp = { };
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

		ComPtr<ID3D12Resource> pResource;
		HR(d3d12Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&pResource)));

		return NewObject<D3D12Resource>(pResource.Get());
	}
	else
	{
		auto it = bufferManager.find(align_size);
		if (it == bufferManager.end())
		{
			it = bufferManager.emplace(align_size, NewObject<D3D12DynamicBufferManager>(align_size)).first;
		}

		return it->second->AllocBuffer();
	}
}

TRefPtr<IRHIResource> D3D12DeviceBundle::CreateImmutableBuffer(size_t sizeInBytes, ERHIResourceStates initialState)
{
	D3D12_RESOURCE_DESC bufferDesc = { };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = (uint64)sizeInBytes;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES heapProp = { };
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	ComPtr<ID3D12Resource> pResource;
	HR(d3d12Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, (D3D12_RESOURCE_STATES)initialState, nullptr, IID_PPV_ARGS(&pResource)));

	return NewObject<D3D12Resource>(pResource.Get());
}

TRefPtr<IRHIResource> D3D12DeviceBundle::CreateTexture2D(ERHITextureFormat format, int32 width, int32 height, ERHIResourceStates initialStates, ERHIResourceFlags flags, const RHITextureClearValue& inClearValue)
{
	D3D12_HEAP_PROPERTIES heapProp{ };
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_DESC desc{ };
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Width = (UINT64)width;
	desc.Height = (UINT)height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = (DXGI_FORMAT)format;
	desc.SampleDesc = { 1, 0 };
	desc.Flags = (D3D12_RESOURCE_FLAGS)flags;

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = (DXGI_FORMAT)inClearValue.Format;
	if (inClearValue.IsColor)
	{
		memcpy(clearValue.Color, &inClearValue.Color.R, sizeof(Color));
	}
	else if (inClearValue.IsDepthStencil)
	{
		clearValue.DepthStencil.Depth = inClearValue.Depth;
		clearValue.DepthStencil.Stencil = inClearValue.Stencil;
	}

	ComPtr<ID3D12Resource> resource;
	HR(d3d12Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc, (D3D12_RESOURCE_STATES)initialStates, inClearValue.IsValid ? &clearValue : nullptr, IID_PPV_ARGS(&resource)));

	return NewObject<D3D12Resource>(resource.Get());
}

TRefPtr<IRHIResource> D3D12DeviceBundle::CreateTexture2D(ERHITextureFormat format, PlatformImage* platformImage)
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
	HR(d3d12Device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource)));

	// Get copyable footprint.
	uint64 totalBytes;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
	d3d12Device->GetCopyableFootprints(&textureDesc, 0, 1, 0, &layout, nullptr, nullptr, &totalBytes);

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
	HR(d3d12Device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadHeap)));

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
	TRefPtr<D3D12DeferredCommandList> deferredCmd = Cast<D3D12DeferredCommandList>(CreateDeferredCommandList());
	deferredCmd->BeginCommand();
	deferredCmd->CommandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
	deferredCmd->CommandList->ResourceBarrier(1, &barrier);
	deferredCmd->EndCommand();

	uint64 issued = resourceGC->IssueNumber();
	resourceGC->AddPendingResource(deferredCmd, issued);
	resourceGC->AddPendingResource(NewObject<D3D12Resource>(uploadHeap.Get()), issued);

	immediateCommandList->ExecuteCommandList(deferredCmd.Get());
	resourceGC->SignalNumber(issued);

	return NewObject<D3D12Resource>(resource.Get());
}

void D3D12DeviceBundle::UpdateTextureGroupView(IRHIShaderResourceView* inView, IRHIResource* const* inResources, size_t count)
{
	if (inView->Count != count)
	{
		SE_LOG(LogRHI, Error, L"UpdateTextureGroupView: SRVs count of ShaderResourceView({0}) is not equals to count of new resources({1}).", inView->Count, count);
		return;
	}

	D3D12IndependentShaderResourceView* d3d12Srv = Cast<D3D12IndependentShaderResourceView>(inView);
	D3D12_SHADER_RESOURCE_VIEW_DESC nullSRV = { };
	nullSRV.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	nullSRV.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	nullSRV.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	for (size_t i = 0; i < count; ++i)
	{
		auto d3d12Resource = Cast<D3D12Resource>(inResources[i]);
		ID3D12Resource* resource = nullptr;
		if (d3d12Resource != nullptr)
		{
			resource = d3d12Resource->Resource;
		}
		
		d3d12Srv->CreateView(i, resource, resource == nullptr ? &nullSRV : nullptr);
	}
}

void D3D12DeviceBundle::InitializeCOM()
{
	HR(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
}

void D3D12DeviceBundle::InitializeDXGI()
{
	UINT flag = 0;
#ifdef _DEBUG
	flag |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	HR(CreateDXGIFactory2(flag, IID_PPV_ARGS(&dxgiFactory)));
}

void D3D12DeviceBundle::InitializeD3D12()
{
	constexpr size_t OfflineDescriptorAllocUnit = 128;

	ComPtr<IDXGIAdapter1> adapter;
	for (int32 i = 0; SUCCEEDED(dxgiFactory->EnumAdapters1((UINT)i, &adapter)); ++i)
	{
		if (!IsAdapterSuitable(adapter.Get()))
		{
			continue;
		}

		if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&d3d12Device))))
		{
			continue;
		}

		if (!IsDeviceSuitable(d3d12Device.Get()))
		{
			continue;
		}

		DXGI_ADAPTER_DESC1 desc = { };
		HR(adapter->GetDesc1(&desc));
		
		SE_LOG(LogD3D12RHI, Verbose, L"That supported feature level 12_1 device named to {0} is selected.", desc.Description);
		break;
	}

	if (!d3d12Device)
	{
		SE_LOG(LogD3D12RHI, Error, "Failed to detect adapter that support feature level 12_1. Create device with software platform.");
		throw HResultException(CO_E_NOT_SUPPORTED);
	}

	// Create core command queue.
	immediateCommandList = NewObject<D3D12ImmediateCommandList>(d3d12Device.Get());

	DXGI_SWAP_CHAIN_DESC1 chainDesc{ };
	chainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	chainDesc.SampleDesc = { 1, 0 };
	chainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	chainDesc.BufferCount = 3;
	chainDesc.Scaling = DXGI_SCALING_STRETCH;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

	HWND hWnd = Application::GetInstance()->GetCoreHwnd();
	ComPtr<IDXGISwapChain1> swapChain;
	HR(dxgiFactory->CreateSwapChainForHwnd(immediateCommandList->CommandQueue, hWnd, &chainDesc, nullptr, nullptr, &swapChain));
	ComPtr<IDXGISwapChain4> swapChain4;
	HR(swapChain.As(&swapChain4));

	this->swapChain = NewObject<D3D12SwapChain>(swapChain4.Get());

	rtvManager = NewObject<D3D12OfflineDescriptorManager>(d3d12Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, OfflineDescriptorAllocUnit);
	dsvManager = NewObject<D3D12OfflineDescriptorManager>(d3d12Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, OfflineDescriptorAllocUnit);
	srvManager = NewObject<D3D12OfflineDescriptorManager>(d3d12Device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, OfflineDescriptorAllocUnit);
}

bool D3D12DeviceBundle::IsAdapterSuitable(IDXGIAdapter1* adapter) const
{
	DXGI_ADAPTER_DESC1 desc{ };
	HR(adapter->GetDesc1(&desc));

	if (desc.Flags != DXGI_ADAPTER_FLAG_NONE)
	{
		// Is remote or software implement.
		return false;
	}

	return true;
}

bool D3D12DeviceBundle::IsDeviceSuitable(ID3D12Device* device) const
{
	UNREFERENCED_PARAMETER(device);

	return true;
}

ComPtr<ID3D12Resource> D3D12DeviceBundle::CreateImmutableBuffer(D3D12_RESOURCE_STATES initialState, const uint8* initialBuffer, size_t sizeInBytes, ERHIResourceFlags flags)
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
	bufferDesc.Flags = (D3D12_RESOURCE_FLAGS)flags;

	D3D12_HEAP_PROPERTIES heap = { };
	heap.Type = D3D12_HEAP_TYPE_DEFAULT;

	ComPtr<ID3D12Resource> resource;
	HR(d3d12Device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource)));

	ComPtr<ID3D12Resource> uploadHeap;
	heap.Type = D3D12_HEAP_TYPE_UPLOAD;
	HR(d3d12Device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadHeap)));

	void* pData;
	HR(uploadHeap->Map(0, nullptr, &pData));
	memcpy(pData, initialBuffer, sizeInBytes);
	uploadHeap->Unmap(0, nullptr);

	D3D12_RESOURCE_BARRIER barrier = { };
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = resource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = initialState;

	TRefPtr<D3D12DeferredCommandList> deferredCmd = Cast<D3D12DeferredCommandList>(CreateDeferredCommandList());
	deferredCmd->BeginCommand();
	deferredCmd->CommandList->CopyResource(resource.Get(), uploadHeap.Get());
	deferredCmd->CommandList->ResourceBarrier(1, &barrier);
	deferredCmd->EndCommand();

	uint64 issued = resourceGC->IssueNumber();
	resourceGC->AddPendingResource(deferredCmd, issued);
	resourceGC->AddPendingResource(NewObject<D3D12Resource>(uploadHeap.Get()), issued);

	immediateCommandList->ExecuteCommandList(deferredCmd.Get());
	resourceGC->SignalNumber(issued);

	return move(resource);
}

void D3D12DeviceBundle::Application_OnSizing(int32 width, int32 height)
{
	SE_LOG(LogD3D12RHI, Verbose, L"Swap chain resized to {0} x {1}", width, height);
	swapChain->ResizeBuffers(width, height);
}