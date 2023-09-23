// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class RHI_API NRHIPipelineState : public NRHIDeviceChild
{
protected:
	NRHIPipelineState() = default;

public:
	virtual ~NRHIPipelineState() noexcept override = default;
};