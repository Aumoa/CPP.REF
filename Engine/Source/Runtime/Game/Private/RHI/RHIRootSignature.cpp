// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIRootSignature.h"

RHIRootSignature::RHIRootSignature(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(std::move(device))
{
}

RHIRootSignature::~RHIRootSignature() noexcept
{
}