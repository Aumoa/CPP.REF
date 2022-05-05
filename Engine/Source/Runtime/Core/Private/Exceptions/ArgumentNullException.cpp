// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/ArgumentNullException.h"
#include "Misc/String.h"

using namespace libty;

ArgumentNullException::ArgumentNullException(StringView argName, std::source_location src)
	: FatalException(String::Format(TEXT("Argument '{}' cannot be nullptr."), argName), nullptr, src)
{
}