// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/ArgumentException.h"
#include "Misc/String.h"

using namespace libty;

ArgumentException::ArgumentException(StringView argName)
	: FatalException(String::Format(TEXT("Invalid argument '{}' detected."), argName))
{
}