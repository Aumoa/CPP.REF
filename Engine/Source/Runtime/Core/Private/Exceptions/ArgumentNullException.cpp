// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/ArgumentNullException.h"
#include "Misc/String.h"

ArgumentNullException::ArgumentNullException(const String& argName)
	: Exception(String::Format(TEXT("Argument '{}' cannot be nullptr."), argName))
{
}