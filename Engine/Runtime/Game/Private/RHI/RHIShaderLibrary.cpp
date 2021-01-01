// Copyright 2020 Aumoa.lib. All right reserved.

#include "RHI/RHIShaderLibrary.h"

#include "Logging/LogMacros.h"
#include "RHI/IRHIShader.h"
#include "RHI/RHICommon.h"

using namespace std;

RHIShaderLibrary::RHIShaderLibrary()
{

}

RHIShaderLibrary::~RHIShaderLibrary()
{

}

bool RHIShaderLibrary::AddShader(TRefPtr<IRHIShader> shader)
{
	if (!shader.IsValid)
	{
		SE_LOG(LogRHI, Error, L"Unexpected error. Shader is invalid.");
		return false;
	}

	auto& mapped_ptr = shaderMap[shader->GetShaderTypeHashCode()];
	if (mapped_ptr.IsValid)
	{
		SE_LOG(LogRHI, Error, "Shader duplication detected on library.");
		return false;
	}

	mapped_ptr = move(shader);
	return true;
}

IRHIShader* RHIShaderLibrary::GetShader(size_t shaderTypeHashCode) const
{
	auto it = shaderMap.find(shaderTypeHashCode);
	if (it == shaderMap.end())
	{
		SE_LOG(LogRHI, Error, "Shader is not contains to library.");
		return nullptr;
	}

	return it->second.Get();
}