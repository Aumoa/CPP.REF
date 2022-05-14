// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/ArgumentNullException.h"
#include "Misc/String.h"

using namespace libty;

ArgumentNullException::ArgumentNullException(String argName)
	: Exception(String::Format(TEXT("Argument '{}' cannot be nullptr."), argName))
{
}