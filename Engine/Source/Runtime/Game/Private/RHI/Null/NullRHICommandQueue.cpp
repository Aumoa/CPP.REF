// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHICommandQueue.h"

NullRHICommandQueue::NullRHICommandQueue(std::shared_ptr<RHIDevice> device)
	: RHICommandQueue(std::move(device))
{
}

NullRHICommandQueue::~NullRHICommandQueue() noexcept
{
}