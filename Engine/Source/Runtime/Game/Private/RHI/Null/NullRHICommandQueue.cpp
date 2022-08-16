// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHICommandQueue.h"
#include "RHI/Null/NullRHIFence.h"

NullRHICommandQueue::NullRHICommandQueue(std::shared_ptr<RHIDevice> device)
	: RHICommandQueue(std::move(device))
{
}

NullRHICommandQueue::~NullRHICommandQueue() noexcept
{
}

void NullRHICommandQueue::Signal(std::shared_ptr<RHIFence> fence, uint64 fenceValue)
{
	auto nFence = std::static_pointer_cast<NullRHIFence>(fence);
	nFence->SetFenceValue(fenceValue);
}

void NullRHICommandQueue::ExecuteCommandLists(std::span<std::shared_ptr<RHICommandList>> cmdLists)
{
	check(cmdLists.size() > 0);
}