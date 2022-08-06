// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceResource.h"

class GAME_API RHIRaytracingPipelineState : public RHIDeviceResource
{
protected:
	RHIRaytracingPipelineState(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHIRaytracingPipelineState() noexcept override;
};