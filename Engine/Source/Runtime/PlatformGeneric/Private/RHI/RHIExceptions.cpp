// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/RHIExceptions.h"

GENERATE_BODY(SRHIException);

SRHIException::SRHIException(std::wstring_view API, std::wstring_view Message, SException* InnerException)
	: Super(Message, InnerException)
	, APIName(API)
{
}

std::wstring_view SRHIException::GetAPIName()
{
	return APIName;
}

GENERATE_BODY(SRHIShaderCompilerException);

SRHIShaderCompilerException::SRHIShaderCompilerException(std::wstring_view API, std::wstring_view Message, SException* InnerException)
	: Super(API, Message, InnerException)
{
}