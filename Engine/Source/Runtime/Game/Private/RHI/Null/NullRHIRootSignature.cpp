// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHIRootSignature.h"

NullRHIRootSignature::NullRHIRootSignature(std::shared_ptr<RHIDevice> device, const RHIRootSignatureDesc& desc)
	: RHIRootSignature(std::move(device))
{
}

NullRHIRootSignature::~NullRHIRootSignature() noexcept
{
}