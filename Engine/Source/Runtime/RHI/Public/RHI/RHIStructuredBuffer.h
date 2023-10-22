// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class RHI_API NRHIStructuredBuffer : public NRHIDeviceChild
{
protected:
	NRHIStructuredBuffer() noexcept = default;

public:
	virtual ~NRHIStructuredBuffer() noexcept override = default;

	virtual size_t GetBufferSize() const = 0;
	virtual void* GetBufferPointer() const = 0;
	virtual Task<> CommitAsync() = 0;
	virtual int64 GetGPUVirtualAddress() const = 0;
};