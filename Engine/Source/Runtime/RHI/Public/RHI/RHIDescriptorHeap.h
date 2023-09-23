// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class RHI_API NRHIDescriptorHeap : public NRHIDeviceChild
{
protected:
	NRHIDescriptorHeap() noexcept = default;

public:
	virtual ~NRHIDescriptorHeap() noexcept override = default;

	virtual void Clear(bool bShrink) = 0;
	virtual void Reserve(size_t Count) = 0;
	virtual size_t GetCount() const = 0;
	virtual size_t GetCapacity() const = 0;
};