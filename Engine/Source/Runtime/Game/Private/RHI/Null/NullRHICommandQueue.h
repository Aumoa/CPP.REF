// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHICommandQueue.h"

class NullRHIDevice;

class NullRHICommandQueue : public RHICommandQueue
{
	friend NullRHIDevice;

private:
	NullRHICommandQueue(std::shared_ptr<RHIDevice> device);

public:
	virtual ~NullRHICommandQueue() noexcept override;

	virtual void Signal(std::shared_ptr<RHIFence> fence, uint64 fenceValue) override;
	virtual void ExecuteCommandLists(std::span<std::shared_ptr<RHICommandList>> cmdLists) override;
};