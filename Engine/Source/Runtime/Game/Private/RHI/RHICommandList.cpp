// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHICommandList.h"

RHICommandList::RHICommandList(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(std::move(device))
{
}

RHICommandList::~RHICommandList() noexcept
{
}