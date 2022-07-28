// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIShaderBytecode.h"

RHIShaderBytecode::RHIShaderBytecode(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(device)
{
}

RHIShaderBytecode::~RHIShaderBytecode() noexcept
{
}