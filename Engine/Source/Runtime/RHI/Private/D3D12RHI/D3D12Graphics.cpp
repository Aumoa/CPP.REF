// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Graphics.h"
#include "D3D12RHI/D3D12CommandQueue.h"
#include "D3D12RHI/D3D12Viewport.h"
#include "D3D12RHI/D3D12Global.h"
#include "D3D12RHI/D3D12Texture2D.h"
#include "D3D12RHI/D3D12CommandSet.h"
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

	// Init AsyncCommands requirements.
	HR(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&AsyncCommandAllocator)));
	HR(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, AsyncCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&AsyncCommandList)));
	HR(AsyncCommandList->Close());
}

std::shared_ptr<NRHICommandQueue> ND3D12Graphics::CreateCommandQueue()
{
	return std::make_shared<ND3D12CommandQueue>(this);
}

std::shared_ptr<NRHIViewport> ND3D12Graphics::CreateViewport(NRHICommandQueue& InCommandQueue, NGenericWindow& InWindow)
{
	auto* Queue = static_cast<ND3D12CommandQueue&>(InCommandQueue).GetQueue();
	HWND hWnd = reinterpret_cast<HWND>(InWindow.GetOSWindowHandle());
	return std::make_shared<ND3D12Viewport>(DXGIFactory.Get(), Queue, hWnd);
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

	co_await ImageSource->CopyPixelsAsync((int32)RowSizeInBytes, (int32)TotalBytes, pBuffer);
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
	});

	co_return std::make_shared<ND3D12Texture2D>(std::move(CommittedResource), Texture2DDesc);
}

std::shared_ptr<NRHICommandSet> ND3D12Graphics::CreateCommandSet()
{
	return std::make_shared<ND3D12CommandSet>();
}

ID3D12Device1* ND3D12Graphics::GetDevice() const
{
	return Device.Get();
}

Task<> ND3D12Graphics::EnqueueGraphicsCommandAsync(Action<ID3D12GraphicsCommandList*> InAction)
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
	if (Fence->GetCompletedValue() < FenceValue)
	{
		HR(Fence->SetEventOnCompletion(FenceValue, hFenceEvent));
		WaitForSingleObject(hFenceEvent, INFINITE);
	}
	check(Fence->GetCompletedValue() >= FenceValue);
	PulseAsyncCommands();
}

void ND3D12Graphics::EndFrame()
{
	ID3D12CommandQueue* Queue = ND3D12Global::GetPrimaryCommandQueue().GetQueue();
	HR(Queue->Signal(Fence.Get(), ++FenceValue));
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
	HR(AsyncCommandList->Reset(AsyncCommandAllocator.Get(), nullptr));

	for (auto& Command : Commands)
	{
		Command(AsyncCommandList.Get());
	}

	HR(AsyncCommandList->Close());

	ID3D12CommandQueue* Queue = ND3D12Global::GetPrimaryCommandQueue().GetQueue();
	ID3D12CommandList* pCommandList = AsyncCommandList.Get();
	Queue->ExecuteCommandLists(1, &pCommandList);
}

#endif