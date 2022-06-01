// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/ArgumentException.h"
#include "Misc/String.h"

ArgumentException::ArgumentException(const String& argName)
	: Exception(String::Format(TEXT("Invalid argument '{}' detected."), argName))
{
}