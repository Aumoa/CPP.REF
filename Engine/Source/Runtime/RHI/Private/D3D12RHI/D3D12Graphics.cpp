// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Graphics.h"
#include "D3D12RHI/D3D12CommandQueue.h"
#include "D3D12RHI/D3D12SwapChain.h"
#include "D3D12RHI/D3D12Viewport.h"
#include "D3D12RHI/D3D12Global.h"
#include "D3D12RHI/D3D12Texture2D.h"
#include "D3D12RHI/D3D12CommandSet.h"
#include "D3D12RHI/D3D12ConstantBuffer.h"
#include "D3D12RHI/D3D12DescriptorHeap.h"
#include "D3D12RHI/D3D12ShaderResourceView.h"
#include "D3D12RHI/D3D12TextFormat.h"
#include "D3D12RHI/D3D12TextLayout.h"
#include "D3D12RHI/D3D12SlateShader.h"
#include "GenericPlatform/GenericWindow.h"

ND3D12Graphics::ND3D12Graphics()
{
}

ND3D12Graphics::~ND3D12Graphics() noexcept
{
}

void ND3D12Graphics::Init()
{
#if !SHIPPING
	if (ComPtr<ID3D12Debug> pDebug; SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebug))))
	{
		pDebug->EnableDebugLayer();
	}
#endif

	HR(CreateDXGIFactory2(0, IID_PPV_ARGS(&DXGIFactory)));

	for (UINT AdapterIndex = 0; ; ++AdapterIndex)
	{
		ComPtr<IDXGIAdapter1> Adapter;
		HRESULT HResult = DXGIFactory->EnumAdapters1(AdapterIndex, &Adapter);
		checkf(HResult != DXGI_ERROR_NOT_FOUND, TEXT("Failed to find suitable adapter."));
		if (FAILED(HResult))
		{
			ReportHResult(HResult);
		}

		DXGI_ADAPTER_DESC1 Desc1;
		HR(Adapter->GetDesc1(&Desc1));

		if (Desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Software adapter does not supported.
			continue;
		}

		HResult = D3D12CreateDevice(Adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&Device));
		if (FAILED(HResult))
		{
			// Cannot create device.
			continue;
		}

		CurrentAdapter = std::move(Adapter);
		break;
	}

	HR(Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));
	hFenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	ComPtr<IUnknown> pUnknown;
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &pUnknown));
	HR(pUnknown.As(&WriteFactory));

	D3D12_COMMAND_QUEUE_DESC PrimaryQueueDesc =
	{
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
		.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH,
	};

	ComPtr<ID3D12CommandQueue> pPrimaryQueue;
	HR(Device->CreateCommandQueue(&PrimaryQueueDesc, IID_PPV_ARGS(&pPrimaryQueue)));
	PrimaryQueue = std::make_shared<ND3D12CommandQueue>(pPrimaryQueue);

	UINT DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if !SHIPPING
	DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HR(D3D11On12CreateDevice(Device.Get(), DeviceFlags, NULL, 0, (IUnknown**)pPrimaryQueue.GetAddressOf(), 1, 0, &Device11, &DeviceContext, nullptr));
	HR(Device11.As(&Device11On12));

	ComPtr<IDXGIDevice> pDXGIDevice;
	HR(Device11On12.As(&pDXGIDevice));

	D2D1_DEBUG_LEVEL D2DDebugLevel = D2D1_DEBUG_LEVEL_NONE;
#if !SHIPPING
	D2DDebugLevel = D2D1_DEBUG_LEVEL_WARNING;
#endif
	D2D1CreateDevice(pDXGIDevice.Get(), D2D1::CreationProperties(D2D1_THREADING_MODE_MULTI_THREADED, D2DDebugLevel, D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS), &D2DDevice);

	// Init AsyncCommands requirements.
	HR(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&AsyncCommandAllocator)));
}

std::shared_ptr<NRHICommandQueue> ND3D12Graphics::GetPrimaryQueue()
{
	return PrimaryQueue;
}

std::shared_ptr<NRHICommandQueue> ND3D12Graphics::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC QueueDesc =
	{
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
		.Priority = (INT)D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
		.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
		.NodeMask = 0
	};

	ComPtr<ID3D12CommandQueue> pQueue;
	HR(Device->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&pQueue)));

	return std::make_shared<ND3D12CommandQueue>(std::move(pQueue));
}

std::shared_ptr<NRHISwapChain> ND3D12Graphics::CreateSwapChain(NRHICommandQueue& InCommandQueue, NGenericWindow& InWindow)
{
	auto* Queue = static_cast<ND3D12CommandQueue&>(InCommandQueue).GetQueue();
	HWND hWnd = reinterpret_cast<HWND>(InWindow.GetOSWindowHandle());
	return std::make_shared<ND3D12SwapChain>(DXGIFactory.Get(), Queue, hWnd);
}

std::shared_ptr<NRHIViewport> ND3D12Graphics::CreateViewport()
{
	return std::make_shared<ND3D12Viewport>(Device.Get());
}

Task<std::shared_ptr<NRHITexture2D>> ND3D12Graphics::CreateTexture2DAsync(std::shared_ptr<NGenericImage> ImageSource)
{
	D3D12_RESOURCE_DESC Texture2DDesc = GetTexture2DDesc(ImageSource->GetSize(), D3D12_RESOURCE_FLAG_NONE);
	ComPtr<ID3D12Resource> CommittedResource;
	HR(Device->CreateCommittedResource(&DefaultHeap, D3D12_HEAP_FLAG_NONE, &Texture2DDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&CommittedResource)));

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT Layouts;
	UINT64 RowSizeInBytes;
	UINT64 TotalBytes;
	Device->GetCopyableFootprints(&Texture2DDesc, 0, 1, 0, &Layouts, nullptr, &RowSizeInBytes, &TotalBytes);

	D3D12_RESOURCE_DESC UploadDesc = GetUploadHeapDesc(TotalBytes);
	ComPtr<ID3D12Resource> UploadResource;
	HR(Device->CreateCommittedResource(&UploadHeap, D3D12_HEAP_FLAG_NONE, &UploadDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&UploadResource)));

	void* pBuffer;
	HR(UploadResource->Map(0, nullptr, &pBuffer));

	co_await ImageSource->CopyPixelsAsync((int32)Layouts.Footprint.RowPitch, (int32)TotalBytes, pBuffer);
	co_await EnqueueGraphicsCommandAsync([&](ID3D12GraphicsCommandList* pCmd)
	{
		D3D12_TEXTURE_COPY_LOCATION LocDst;
		LocDst.pResource = CommittedResource.Get();
		LocDst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		LocDst.SubresourceIndex = 0;

		D3D12_TEXTURE_COPY_LOCATION LocSrc;
		LocSrc.pResource = UploadResource.Get();
		LocSrc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		LocSrc.PlacedFootprint = Layouts;

		pCmd->CopyTextureRegion(&LocDst, 0, 0, 0, &LocSrc, nullptr);

		return false;
	});

	co_return std::make_shared<ND3D12Texture2D>(std::move(CommittedResource), Texture2DDesc);
}

std::shared_ptr<NRHICommandSet> ND3D12Graphics::CreateCommandSet()
{
	return std::make_shared<ND3D12CommandSet>();
}

std::shared_ptr<NRHISlateShader> ND3D12Graphics::CreateSlateShader()
{
	return std::make_shared<ND3D12SlateShader>(*Device.Get());
}

std::shared_ptr<NRHIConstantBuffer> ND3D12Graphics::CreateConstantBuffer()
{
	return std::make_shared<ND3D12ConstantBuffer>(*Device.Get());
}

std::shared_ptr<NRHIDescriptorHeap> ND3D12Graphics::CreateDescriptorHeap()
{
	return std::make_shared<ND3D12DescriptorHeap>(*Device.Get());
}

std::shared_ptr<NRHIShaderResourceView> ND3D12Graphics::CreateShaderResourceView(size_t InNumViews)
{
	return std::make_shared<ND3D12ShaderResourceView>(*Device.Get(), InNumViews);
}

std::shared_ptr<NRHITextFormat> ND3D12Graphics::CreateTextFormat(String FontFamilyName, float FontSize, bool bBold, bool bItalic)
{
	return std::make_shared<ND3D12TextFormat>(FontFamilyName, FontSize, bBold, bItalic);
}

std::shared_ptr<NRHITextLayout> ND3D12Graphics::CreateTextLayout(std::shared_ptr<NRHITextFormat> InTextFormat, String InText)
{
	IDWriteTextFormat* pTextFormat = static_cast<ND3D12TextFormat*>(InTextFormat.get())->GetTextFormat();
	ComPtr<IDWriteTextLayout> pTextLayout;
	HR(WriteFactory->CreateTextLayout(InText.c_str(), (UINT32)InText.length(), pTextFormat, D3D12_FLOAT32_MAX, D3D12_FLOAT32_MAX, &pTextLayout));
	auto Layout = std::make_shared<ND3D12TextLayout>(std::static_pointer_cast<ND3D12TextFormat>(std::move(InTextFormat)), std::move(pTextLayout), InText);
	return Layout;
}

Task<> ND3D12Graphics::EnqueueGraphicsCommandAsync(Func<ID3D12GraphicsCommandList*, bool> InAction)
{
	std::unique_lock ScopedLock(AsyncLock);
	AsyncCommands.emplace_back(std::move(InAction));
	auto& Pulse = AsyncPulse[AsyncIndex % 2];
	if (Pulse.IsValid() == false)
	{
		Pulse = TaskCompletionSource<>::Create();
	}
	return Pulse.GetTask();
}

std::unique_ptr<NRHIGraphics> ND3D12Graphics::GenerateGraphics()
{
	return std::make_unique<ND3D12Graphics>();
}

void ND3D12Graphics::BeginFrame()
{
	SyncFrame();
	check(Fence->GetCompletedValue() >= FenceValue);
	PulseAsyncCommands();
}

void ND3D12Graphics::EndFrame()
{
	ID3D12CommandQueue* Queue = ND3D12Global::GetPrimaryCommandQueue().GetQueue();
	HR(Queue->Signal(Fence.Get(), ++FenceValue));
}

void ND3D12Graphics::SyncFrame()
{
	if (Fence->GetCompletedValue() < FenceValue)
	{
		HR(Fence->SetEventOnCompletion(FenceValue, hFenceEvent));
		WaitForSingleObject(hFenceEvent, INFINITE);
	}
}

ComPtr<ID2D1DeviceContext> ND3D12Graphics::CreateDeviceContext2D() const
{
	ComPtr<ID2D1DeviceContext> Context;
	HR(D2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &Context));
	return Context;
}

void ND3D12Graphics::PulseAsyncCommands()
{
	std::unique_lock ScopedLock(AsyncLock);
	size_t OtherIndex = ++AsyncIndex % 2;
	auto Pulse = std::move(AsyncPulse[OtherIndex]);
	auto Commands = std::move(AsyncCommands);
	ScopedLock.unlock();

	// Set result for previous actions.
	if (Pulse.IsValid())
	{
		Pulse.SetResult();
	}

	// Execute for current actions.
	HR(AsyncCommandAllocator->Reset());
	if (Commands.empty())
	{
		return;
	}

	ComPtr<ID3D12GraphicsCommandList>* pCurrentCmd = nullptr;
	size_t CmdIndex = 0;
	ID3D12CommandQueue* Queue = ND3D12Global::GetPrimaryCommandQueue().GetQueue();
	Action<> CloseCommandAndNext = [&]()
	{
		HR((*pCurrentCmd)->Close());
		ID3D12CommandList* p = pCurrentCmd->Get();
		Queue->ExecuteCommandLists(1, &p);
		pCurrentCmd = nullptr;
		++CmdIndex;
	};

	for (auto& Command : Commands)
	{
		if (!pCurrentCmd)
		{
			if (AsyncCommandLists.size() > CmdIndex)
			{
				pCurrentCmd = &AsyncCommandLists[CmdIndex];
				HR((*pCurrentCmd)->Reset(AsyncCommandAllocator.Get(), nullptr));
			}
			else
			{
				pCurrentCmd = &AsyncCommandLists.emplace_back();
				HR(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, AsyncCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&*pCurrentCmd)));
			}
		}

		if (Command(pCurrentCmd->Get()) == true)
		{
			CloseCommandAndNext();
		}
	}

	if (pCurrentCmd)
	{
		CloseCommandAndNext();
	}
}

#endif