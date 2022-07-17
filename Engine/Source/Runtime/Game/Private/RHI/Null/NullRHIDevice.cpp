// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHIDevice.h"
#include "RHI/Null/NullRHIFactory.h"
#include "RHI/Null/NullRHICommandQueue.h"
#include "RHI/Null/NullRHISwapChain.h"

NullRHIDevice::NullRHIDevice(std::shared_ptr<RHIFactory> factory)
	: RHIDevice(factory)
{
}

NullRHIDevice::~NullRHIDevice() noexcept
{
}

std::shared_ptr<RHICommandQueue> NullRHIDevice::CreateCommandQueue()
{
	return std::shared_ptr<RHICommandQueue>(new NullRHICommandQueue(shared_from_this()));
}

std::shared_ptr<RHISwapChain> NullRHIDevice::CreateSwapChain(std::shared_ptr<RHICommandQueue> queue, IPlatformWindow* drawingWindow)
{
	return std::shared_ptr<RHISwapChain>(new NullRHISwapChain(shared_from_this()));
}