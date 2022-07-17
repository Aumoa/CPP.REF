// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDevice.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIFactory;

class GAME_API WindowsRHIDevice : public RHIDevice
{
	friend WindowsRHIFactory;

private:
	ComPtr<IDXGIAdapter1> _physicalDevice;
	ComPtr<ID3D12Device> _device;

private:
	WindowsRHIDevice(std::shared_ptr<WindowsRHIFactory> factory);

public:
	virtual ~WindowsRHIDevice() noexcept override;

	inline ID3D12Device* GetDevice() const { return _device.Get(); }

	virtual std::shared_ptr<RHICommandQueue> CreateCommandQueue() override;
	virtual std::shared_ptr<RHISwapChain> CreateSwapChain(std::shared_ptr<RHICommandQueue> queue, IPlatformWindow* drawingWindow) override;
};

#endif