// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHISwapChain.h"

RHISwapChain::RHISwapChain(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(std::move(device))
{
}

RHISwapChain::~RHISwapChain() noexcept
{
}