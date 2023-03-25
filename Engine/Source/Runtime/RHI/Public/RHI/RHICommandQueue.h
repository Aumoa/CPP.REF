// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class RHI_API NRHICommandQueue : public NRHIDeviceChild
{
protected:
	NRHICommandQueue();

public:
	virtual ~NRHICommandQueue() noexcept override;
};