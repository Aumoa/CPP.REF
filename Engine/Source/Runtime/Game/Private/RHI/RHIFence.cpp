// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIFence.h"

RHIFence::RHIFence(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(std::move(device))
{
}

RHIFence::~RHIFence() noexcept
{
}