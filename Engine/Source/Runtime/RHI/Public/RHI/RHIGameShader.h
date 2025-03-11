// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class RHI_API NRHIGameShader : public NRHIDeviceChild
{
protected:
	NRHIGameShader() = default;

public:
	virtual ~NRHIGameShader() noexcept override = default;
};