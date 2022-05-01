// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/ArgumentException.h"
#include "Misc/String.h"

using namespace libty;

ArgumentException::ArgumentException(std::string_view argName, std::source_location src)
	: FatalException(String::Format("Invalid argument '{}' detected.", argName), nullptr, src)
{
}