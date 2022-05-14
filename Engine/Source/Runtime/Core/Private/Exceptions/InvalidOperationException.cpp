// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/InvalidOperationException.h"

using namespace libty;

InvalidOperationException::InvalidOperationException(String message, std::exception_ptr innerException)
	: Exception(message, innerException)
{
}

InvalidOperationException::InvalidOperationException(std::exception_ptr innerException)
	: Exception(TEXT("Invalid operation detected"), innerException)
{
}