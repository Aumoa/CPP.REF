// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIRaytracingPipelineState.h"

RHIRaytracingPipelineState::RHIRaytracingPipelineState(std::shared_ptr<RHIDevice> device)
	: RHIDeviceResource(std::move(device))
{
}

RHIRaytracingPipelineState::~RHIRaytracingPipelineState() noexcept
{
}