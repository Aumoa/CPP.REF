// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceChild.h"

class RHI_API NRHIConstantBuffer : public NRHIDeviceChild
{
protected:
	NRHIConstantBuffer() noexcept = default;

public:
	virtual ~NRHIConstantBuffer() noexcept override = default;

	virtual void Reserve(size_t InBufferSize) = 0;
	virtual size_t GetBufferSize() const = 0;
	virtual void* GetBufferPointer() const = 0;
	virtual int64 GetGPUVirtualAddress() const = 0;
};