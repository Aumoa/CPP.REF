// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/ArgumentNullException.h"
#include "Misc/String.h"

using namespace libty;

ArgumentNullException::ArgumentNullException(std::string_view argName, std::source_location src)
	: FatalException(String::Format("Argument '{}' cannot be nullptr.", argName), nullptr, src)
{
}