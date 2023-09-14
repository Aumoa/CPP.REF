// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class NRHIViewport;

class RHI_API NRHICommandSet : public NRHIDeviceChild
{
protected:
	NRHICommandSet() = default;

public:
	virtual ~NRHICommandSet() noexcept override = default;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void BeginRender(const NRHIViewport& InViewport, bool bClear) = 0;
	virtual void EndRender(const NRHIViewport& InViewport) = 0;
};