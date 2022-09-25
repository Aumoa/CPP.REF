// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHIDevice.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIFactory.h"

WindowsRHIDevice::WindowsRHIDevice(std::shared_ptr<WindowsRHIFactory> factory)
	: RHIDevice(factory)
{
	if (factory->IsDebugLayerEnabled())
	{
		if (ComPtr<ID3D12Debug> debugInterface; ensureMsgf(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))), TEXT("[WindowsRHI] Debugging layer enabled, but cannot query debug interface for physical device.")))
		{
			debugInterface->EnableDebugLayer();
		}
	}

	IDXGIFactory4* dxgiFactory = factory->GetFactory();
	HR(dxgiFactory->EnumAdapters1(0, &_physicalDevice));
	HR(D3D12CreateDevice(_physicalDevice.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&_device)));
}

WindowsRHIDevice::~WindowsRHIDevice() noexcept
{
}

#endif