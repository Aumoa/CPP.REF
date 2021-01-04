// Copyright 2020 Aumoa.lib. All right reserved.

#include "RHI/RHIShaderDescription.h"

using namespace std;

RHIShaderBytecode::RHIShaderBytecode()
	: pShaderBytecode(nullptr)
	, BytecodeLength(0)
{

}

RHIShaderBytecode::RHIShaderBytecode(const void* pShaderBytecode, size_t bytecodeLength)
	: pShaderBytecode(pShaderBytecode)
	, BytecodeLength(bytecodeLength)
{

}

RHIShaderBytecode::RHIShaderBytecode(span<const uint8> shaderBytecode) : This(shaderBytecode.data(), shaderBytecode.size_bytes())
{

}

RHIShaderBytecode::~RHIShaderBytecode()
{

}

bool RHIShaderBytecode::IsValid_get() const
{
	return pShaderBytecode != nullptr && BytecodeLength != 0;
}

RHIShaderDescription::RHIShaderDescription()
{

}

RHIShaderDescription::~RHIShaderDescription()
{

}