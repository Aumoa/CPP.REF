// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class RHI_API NRHIRootSignature : public NRHIDeviceChild
{
protected:
	NRHIRootSignature() = default;

public:
	virtual ~NRHIRootSignature() noexcept override = default;
};