// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NRHIGraphics;

class RHI_API NRHIDeviceChild : public NonCopyable
{
protected:
	NRHIDeviceChild();

public:
	virtual ~NRHIDeviceChild() noexcept;

	virtual NRHIGraphics* GetDevice() const = 0;
};