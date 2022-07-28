// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIPipelineState.h"

RHIPipelineState::RHIPipelineState(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(std::move(device))
{
}

RHIPipelineState::~RHIPipelineState() noexcept
{
}