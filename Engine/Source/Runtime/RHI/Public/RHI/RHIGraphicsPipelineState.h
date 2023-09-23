// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIPipelineState.h"

class RHI_API NRHIGraphicsPipelineState : public NRHIPipelineState
{
protected:
	NRHIGraphicsPipelineState() noexcept = default;

public:
	virtual ~NRHIGraphicsPipelineState() noexcept override = default;
};