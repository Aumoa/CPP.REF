// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIResource.h"

RHIResource::RHIResource(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(std::move(device))
{
}

RHIResource::~RHIResource() noexcept
{
}