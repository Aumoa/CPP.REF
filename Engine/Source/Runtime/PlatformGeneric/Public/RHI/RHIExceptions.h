// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Exceptions/Exception.h"

class PLATFORMGENERIC_API SRHIException : public SException
{
	GENERATED_BODY(SRHIException)

private:
	std::wstring APIName;

public:
	SRHIException(std::wstring_view API, std::wstring_view Message, SException* InnerException = nullptr);

	std::wstring_view GetAPIName();
};

class PLATFORMGENERIC_API SRHIShaderCompilerException : public SRHIException
{
	GENERATED_BODY(SRHIShaderCompilerException)

public:
	SRHIShaderCompilerException(std::wstring_view API, std::wstring_view Message, SException* InnerException = nullptr);
};