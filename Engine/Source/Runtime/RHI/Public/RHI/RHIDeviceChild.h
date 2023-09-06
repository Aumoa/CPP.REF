// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NRHIGraphics;

class RHI_API NRHIDeviceChild
{
	NRHIDeviceChild(const NRHIDeviceChild&) = delete;
	NRHIDeviceChild(NRHIDeviceChild&&) = delete;

protected:
	NRHIDeviceChild();

public:
	virtual ~NRHIDeviceChild() noexcept;
};