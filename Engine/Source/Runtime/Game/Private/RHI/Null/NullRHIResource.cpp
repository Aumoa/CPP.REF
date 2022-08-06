// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHIResource.h"

NullRHIResource::NullRHIResource(std::shared_ptr<RHIDevice> device)
	: RHIResource(std::move(device))
{
}

NullRHIResource::~NullRHIResource() noexcept
{
}