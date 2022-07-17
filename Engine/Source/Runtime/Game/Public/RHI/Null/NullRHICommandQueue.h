// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHICommandQueue.h"

class NullRHIDevice;

class GAME_API NullRHICommandQueue : public RHICommandQueue
{
	friend NullRHIDevice;

private:
	NullRHICommandQueue(std::shared_ptr<RHIDevice> device);

public:
	virtual ~NullRHICommandQueue() noexcept override;
};