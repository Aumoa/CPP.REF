// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHIShaderBytecode.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIDevice.h"

std::shared_ptr<RHIShaderBytecode> WindowsRHIDevice::CreateShaderBytecode(const void* pShaderBytecode, size_t bytecodeLength)
{
	return std::shared_ptr<RHIShaderBytecode>(new WindowsRHIShaderBytecode(SharedFromThis(), pShaderBytecode, bytecodeLength));
}

WindowsRHIShaderBytecode::WindowsRHIShaderBytecode(std::shared_ptr<WindowsRHIDevice> device, const void* pShaderBytecode, size_t bytecodeLength)
	: RHIShaderBytecode(std::move(device))
{
	HR(::D3DCreateBlob(bytecodeLength, &_blob));
	memcpy(_blob->GetBufferPointer(), pShaderBytecode, bytecodeLength);
}

WindowsRHIShaderBytecode::~WindowsRHIShaderBytecode() noexcept
{
}

size_t WindowsRHIShaderBytecode::GetBufferSize() const noexcept
{
	return _blob->GetBufferSize();
}

const void* WindowsRHIShaderBytecode::GetBufferPointer() const noexcept
{
	return _blob->GetBufferPointer();
}

#endif