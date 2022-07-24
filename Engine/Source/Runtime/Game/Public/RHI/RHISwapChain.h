// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceResource.h"

class GAME_API RHISwapChain : public RHIDeviceResource
{
protected:
	RHISwapChain(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHISwapChain() noexcept override;

	virtual void Present() = 0;
};