// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

class FormatException : public Exception
{
public:
	FormatException(const String& message, std::exception_ptr innerException = {})
		: Exception(message, innerException)
	{
	}
};

[[noreturn]]
constexpr void String::ThrowFormatError(bool b)
{
	if (b)
	{
		throw FormatException(TEXT("Invalid number format."));
	}
}