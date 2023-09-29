// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NRHIGraphics;

class RHI_API NRHIDeviceChild : public std::enable_shared_from_this<NRHIDeviceChild>
{
	NRHIDeviceChild(const NRHIDeviceChild&) = delete;
	NRHIDeviceChild(NRHIDeviceChild&&) = delete;

protected:
	NRHIDeviceChild() = default;

public:
	virtual ~NRHIDeviceChild() noexcept = default;
};