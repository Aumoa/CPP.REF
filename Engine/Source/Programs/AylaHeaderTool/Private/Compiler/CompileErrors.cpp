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

String CompileErrors::Colon()
{
	return TEXT("error A004: Expected: colon.");
}

String CompileErrors::IllegalKeyword(String InKeyword)
{
	return String::Format(TEXT("error A005: Unexpected keyword '{}'."), InKeyword);
}

String CompileErrors::MultipleInheritance()
{
	return TEXT("error A006: Multiple inheritance is not allowed.");
}

String CompileErrors::IllegalAccess()
{
	return TEXT("error A007: Unexpected access modifier.");
}