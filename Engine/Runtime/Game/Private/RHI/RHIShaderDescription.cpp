// Copyright 2020 Aumoa.lib. All right reserved.

#include "RHI/RHIShaderDescription.h"

#include "RHI/RHITextureFormat.h"

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
	: DSVFormat(ERHITextureFormat::Unknown)
{

}

RHIShaderDescription::~RHIShaderDescription()
{

}

#define FAST_RETURN(VarName, Exp)\
bool VarName = Exp;\
if (!VarName)\
{\
	return VarName;\
}

bool RHIShaderDescription::IsValid_get() const
{
	// Shader name cannot be empty.
	if (!ShaderName.IsValid)
	{
		return false;
	}

	// Count of RTV formats is now supports up to 8.
	if (RTVFormats.size() > 8)
	{
		return false;
	}

	// RTV formats cannot contain Unknown or typeless format.
	for (size_t i = 0; i < RTVFormats.size(); ++i)
	{
		if (!IsColorFormat(RTVFormats[i]))
		{
			return false;
		}
	}

	return true;
}