// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDevice.h"

class NullRHIFactory;

class GAME_API NullRHIDevice : public RHIDevice
{
	friend NullRHIFactory;

private:
	NullRHIDevice(std::shared_ptr<RHIFactory> factory);

public:
	virtual ~NullRHIDevice() noexcept override;

	virtual std::shared_ptr<RHICommandQueue> CreateCommandQueue() override;
	virtual std::shared_ptr<RHISwapChain> CreateSwapChain(std::shared_ptr<RHICommandQueue> queue, IPlatformWindow* drawingWindow) override;
	virtual std::shared_ptr<RHIFence> CreateFence() override;
};