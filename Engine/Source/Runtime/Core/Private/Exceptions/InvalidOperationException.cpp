// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/InvalidOperationException.h"

using namespace libty;

InvalidOperationException::InvalidOperationException(std::exception_ptr innerException, std::source_location src)
	: FatalException("Invalid operation detected", innerException, src)
{
}

InvalidOperationException::InvalidOperationException(std::string_view message, std::exception_ptr innerException, std::source_location src)
	: FatalException(message, innerException, src)
{
}