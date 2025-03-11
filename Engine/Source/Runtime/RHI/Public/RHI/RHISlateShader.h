// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class RHI_API NRHISlateShader : public NRHIDeviceChild
{
protected:
	NRHISlateShader() = default;

public:
	virtual ~NRHISlateShader() noexcept override = default;
};