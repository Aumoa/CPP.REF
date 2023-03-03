// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Graphics.h"
#include "D3D12RHI/D3D12CommandQueue.h"

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
		HRESULT Result = DXGIFactory->EnumAdapters1(AdapterIndex, &Adapter);
		if (Result == DXGI_ERROR_NOT_FOUND)
		{
			Log::Fatal(LogD3D12, TEXT("Failed to find suitable adapter."));
		}
		else if (FAILED(Result))
		{
			ReportHResult(Result);
		}

		DXGI_ADAPTER_DESC1 Desc1;
		HR(Adapter->GetDesc1(&Desc1));

		if (Desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Software adapter does not supported.
			continue;
		}

		Result = D3D12CreateDevice(Adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&Device));
		if (FAILED(Result))
		{
			// Cannot create device.
			continue;
		}

		CurrentAdapter = std::move(Adapter);
		break;
	}
}

std::shared_ptr<NRHICommandQueue> ND3D12Graphics::CreateCommandQueue()
{
	return std::make_shared<ND3D12CommandQueue>(this);
}

ID3D12Device* ND3D12Graphics::GetDevice() const
{
	return Device.Get();
}

std::unique_ptr<NRHIGraphics> ND3D12Graphics::GenerateGraphics()
{
	return std::make_unique<ND3D12Graphics>();
}

#endif