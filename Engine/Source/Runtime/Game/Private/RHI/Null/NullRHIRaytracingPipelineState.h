// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIRaytracingPipelineState.h"

class NullRHIDevice;

class NullRHIRaytracingPipelineState : public RHIRaytracingPipelineState
{
	friend NullRHIDevice;

private:
	NullRHIRaytracingPipelineState(std::shared_ptr<RHIDevice> device);

public:
	virtual ~NullRHIRaytracingPipelineState() noexcept override;

	virtual const void* GetShaderIdentifier(const String& functionName) const override;
};