// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class NRHICommandSet;

class RHI_API NRHICommandQueue : public NRHIDeviceChild
{
protected:
	NRHICommandQueue() = default;

public:
	virtual ~NRHICommandQueue() noexcept override = default;

	virtual void ExecuteCommandSets(std::span<NRHICommandSet const* const> InCommandSets) = 0;
};