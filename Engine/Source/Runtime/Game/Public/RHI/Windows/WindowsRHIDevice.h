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

	inline ID3D12Device* GetDevice() const noexcept { return _device.Get(); }

	virtual std::shared_ptr<RHICommandQueue> CreateCommandQueue() override;
	virtual std::shared_ptr<RHISwapChain> CreateSwapChain(std::shared_ptr<RHICommandQueue> queue, IPlatformWindow* drawingWindow) override;
	virtual std::shared_ptr<RHIFence> CreateFence() override;

private:
	inline std::shared_ptr<WindowsRHIDevice> SharedFromThis() noexcept
	{
		return std::static_pointer_cast<WindowsRHIDevice>(shared_from_this());
	}
};

#endif