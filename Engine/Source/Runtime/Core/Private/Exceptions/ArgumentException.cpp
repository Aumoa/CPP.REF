// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/ArgumentException.h"
#include "Misc/String.h"

using namespace libty;

ArgumentException::ArgumentException(String argName)
	: Exception(String::Format(TEXT("Invalid argument '{}' detected."), argName))
{
}