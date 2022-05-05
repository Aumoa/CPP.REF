// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/InvalidOperationException.h"

using namespace libty;

InvalidOperationException::InvalidOperationException(StringView message, std::exception_ptr innerException, std::source_location src)
	: FatalException(message, innerException, src)
{
}

InvalidOperationException::InvalidOperationException(std::exception_ptr innerException, std::source_location src)
	: FatalException(TEXT("Invalid operation detected"), innerException, src)
{
}