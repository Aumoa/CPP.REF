// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class RHI_API NRHITexture2D : public NRHIDeviceChild
{
protected:
	NRHITexture2D() = default;

public:
	virtual ~NRHITexture2D() noexcept override = default;
};