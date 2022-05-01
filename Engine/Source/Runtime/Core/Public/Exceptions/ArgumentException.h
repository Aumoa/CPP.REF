// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FatalException.h"

namespace libty::inline Core
{
	class CORE_API ArgumentException : public FatalException
	{
	public:
		ArgumentException(std::string_view argName, std::source_location src = std::source_location::current());
	};
}