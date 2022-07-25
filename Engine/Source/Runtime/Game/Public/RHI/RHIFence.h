// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceResource.h"

class GAME_API RHIFence : public RHIDeviceResource
{
protected:
	RHIFence(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHIFence() noexcept override;

	virtual uint64 GetCompletedValue() const = 0;
	virtual void Wait(uint64 fenceValue) = 0;
};