// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIViewport.h"

class RHI_API NRHISwapChain : public NRHIViewport
{
protected:
	NRHISwapChain() = default;

public:
	virtual void Present() = 0;
};