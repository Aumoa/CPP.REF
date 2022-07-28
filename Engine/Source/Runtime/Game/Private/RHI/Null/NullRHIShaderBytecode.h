// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIShaderBytecode.h"

class NullRHIDevice;

class NullRHIShaderBytecode : public RHIShaderBytecode
{
	friend NullRHIDevice;

private:
	std::vector<void*> _blob;

private:
	NullRHIShaderBytecode(std::shared_ptr<RHIDevice> device, const void* pShaderBytecode, size_t bytecodeLength);

public:
	virtual ~NullRHIShaderBytecode() noexcept override;

	virtual size_t GetBufferSize() const noexcept override;
	virtual const void* GetBufferPointer() const noexcept override;
};