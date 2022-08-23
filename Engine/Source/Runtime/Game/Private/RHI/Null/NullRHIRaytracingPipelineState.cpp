// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHIRaytracingPipelineState.h"

NullRHIRaytracingPipelineState::NullRHIRaytracingPipelineState(std::shared_ptr<RHIDevice> device)
	: RHIRaytracingPipelineState(std::move(device))
{
}

NullRHIRaytracingPipelineState::~NullRHIRaytracingPipelineState() noexcept
{
}

const void* NullRHIRaytracingPipelineState::GetShaderIdentifier(const String& functionName) const
{
	return nullptr;
}