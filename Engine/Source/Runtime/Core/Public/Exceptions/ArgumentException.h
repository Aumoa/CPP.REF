// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

class CORE_API ArgumentException : public Exception
{
public:
	ArgumentException(const String& argName);
};

[[noreturn]]
constexpr void String::ThrowArgumentError(bool b)
{
	if (b)
	{
		throw ArgumentException(TEXT("Invalid number argument."));
	}
}