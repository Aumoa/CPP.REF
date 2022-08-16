// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceResource.h"

class RHIFence;
class RHICommandList;

class GAME_API RHICommandQueue : public RHIDeviceResource
{
protected:
	RHICommandQueue(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHICommandQueue() noexcept override;

	virtual void Signal(std::shared_ptr<RHIFence> fence, uint64 fenceValue) = 0;
	virtual void ExecuteCommandLists(std::span<std::shared_ptr<RHICommandList>> cmdLists) = 0;
};