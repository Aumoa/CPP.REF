// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHISwapChain.h"

NullRHISwapChain::NullRHISwapChain(std::shared_ptr<RHIDevice> device)
	: RHISwapChain(std::move(device))
{
}

NullRHISwapChain::~NullRHISwapChain() noexcept
{
}

void NullRHISwapChain::Present()
{
}