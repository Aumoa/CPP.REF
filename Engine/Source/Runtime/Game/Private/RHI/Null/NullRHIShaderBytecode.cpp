// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHIShaderBytecode.h"

NullRHIShaderBytecode::NullRHIShaderBytecode(std::shared_ptr<RHIDevice> device, const void* pShaderBytecode, size_t bytecodeLength)
	: RHIShaderBytecode(std::move(device))
	, _blob(bytecodeLength)
{
	memcpy(_blob.data(), pShaderBytecode, bytecodeLength);
}

NullRHIShaderBytecode::~NullRHIShaderBytecode() noexcept
{
}

size_t NullRHIShaderBytecode::GetBufferSize() const noexcept
{
	return _blob.size();
}

const void* NullRHIShaderBytecode::GetBufferPointer() const noexcept
{
	return _blob.data();
}