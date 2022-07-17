// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIDeviceResource.h"
#include "RHI/RHIDevice.h"

RHIDeviceResource::RHIDeviceResource(std::shared_ptr<RHIDevice> device)
	: _device(std::move(device))
{
}

RHIDeviceResource::~RHIDeviceResource() noexcept
{
}

std::shared_ptr<RHIDevice> RHIDeviceResource::GetDevice() const
{
	return _device;
}