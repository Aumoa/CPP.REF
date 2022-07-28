// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIShaderBytecode.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIDevice;

class WindowsRHIShaderBytecode : public RHIShaderBytecode
{
	friend WindowsRHIDevice;

private:
	ComPtr<ID3DBlob> _blob;

private:
	WindowsRHIShaderBytecode(std::shared_ptr<WindowsRHIDevice> device, const void* pShaderBytecode, size_t bytecodeLength);

public:
	virtual ~WindowsRHIShaderBytecode() noexcept override;

	virtual size_t GetBufferSize() const noexcept override;
	virtual const void* GetBufferPointer() const noexcept override;
};

#endif