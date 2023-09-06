// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Graphics.h"
#include "D3D12RHI/D3D12CommandQueue.h"
#include "D3D12RHI/D3D12Viewport.h"
#include "RHI/RHIGlobal.h"
#include "GenericPlatform/GenericWindow.h"

ND3D12Graphics::ND3D12Graphics()
{
}

ND3D12Graphics::~ND3D12Graphics() noexcept
{
}

void ND3D12Graphics::Init()
{
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
}

std::shared_ptr<NRHICommandQueue> ND3D12Graphics::CreateCommandQueue()
{
	return std::make_shared<ND3D12CommandQueue>(this);
}

std::shared_ptr<NRHIViewport> ND3D12Graphics::CreateViewport(NRHICommandQueue* InCommandQueue, NGenericWindow* InWindow)
{
	auto* Queue = static_cast<ND3D12CommandQueue*>(InCommandQueue)->GetQueue();
	HWND hWnd = reinterpret_cast<HWND>(InWindow->GetOSWindowHandle());
	return std::make_shared<ND3D12Viewport>(DXGIFactory.Get(), Queue, hWnd);
}

ID3D12Device1* ND3D12Graphics::GetDevice() const
{
	return Device.Get();
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
}

void ND3D12Graphics::EndFrame()
{
	ID3D12CommandQueue* Queue = static_cast<ND3D12CommandQueue*>(NRHIGlobal::GetPrimaryCommandQueue())->GetQueue();
	HR(Queue->Signal(Fence.Get(), ++FenceValue));
}

#endif