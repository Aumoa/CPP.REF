// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHIFactory.h"
#include "RHI/Null/NullRHIDevice.h"

NullRHIFactory::NullRHIFactory() : RHIFactory(false)
{
}

NullRHIFactory::~NullRHIFactory()
{
}

std::shared_ptr<RHIDevice> NullRHIFactory::CreateDevice()
{
	return std::shared_ptr<RHIDevice>(new NullRHIDevice(shared_from_this()));
}