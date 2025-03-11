// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class RHI_API NRHITextFormat : public NRHIDeviceChild
{
protected:
	NRHITextFormat() = default;

public:
	virtual ~NRHITextFormat() noexcept override = default;
};