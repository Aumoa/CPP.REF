// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class RHI_API NRHIShader : public NRHIDeviceChild
{
protected:
	NRHIShader() = default;

public:
	virtual ~NRHIShader() noexcept override = default;
};