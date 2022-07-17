// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHICommandQueue.h"

RHICommandQueue::RHICommandQueue(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(std::move(device))
{
}

RHICommandQueue::~RHICommandQueue() noexcept
{
}