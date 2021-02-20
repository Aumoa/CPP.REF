// Copyright 2020-2021 Aumoa.lib. All right reserved.

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

void RHIShaderLibrary::SetShader(size_t inShaderIndex, TRefPtr<IRHIShader> inShader)
{
	if (shaders.size() <= inShaderIndex)
	{
		shaders.resize(inShaderIndex + 1);
	}

	shaders[inShaderIndex] = move(inShader);
}

IRHIShader* RHIShaderLibrary::GetShader(size_t inShaderIndex) const
{
	if (inShaderIndex >= shaders.size())
	{
		SE_LOG(LogRHI, Error, L"Shader index {0} is not contains to shader library.", inShaderIndex);
		return nullptr;
	}

	return shaders[inShaderIndex].Get();
}