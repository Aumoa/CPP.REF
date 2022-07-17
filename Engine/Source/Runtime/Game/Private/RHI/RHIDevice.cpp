// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIDevice.h"
#include "RHI/RHIFactory.h"

RHIDevice::RHIDevice(std::shared_ptr<RHIFactory> factory)
	: _factory(std::move(factory))
{
}

RHIDevice::~RHIDevice() noexcept
{
}

std::shared_ptr<RHIFactory> RHIDevice::GetFactory() const
{
	return _factory;
}