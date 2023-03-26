// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Compiler/CompileErrors.h"

String CompileErrors::InvalidClassKey(String InClassKey, String InRequired)
{
	return String::Format(TEXT("error A001: Invalid class key('{}') detected. Expected class key: '{}'"), InClassKey, InRequired);
}

String CompileErrors::IllegalScope(bool bOpen)
{
	return String::Format(TEXT("error A002: Illegal class scope detected. Expected: '{}'"), (bOpen ? '{' : '}'));
}

String CompileErrors::GeneratedBody()
{
	return TEXT("error A003: GENERATED_BODY() not found in first of class declaration.");
}