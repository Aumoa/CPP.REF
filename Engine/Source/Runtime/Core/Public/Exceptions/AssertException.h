// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FatalException.h"

namespace libty::inline Core
{
	class CORE_API AssertException : public FatalException
	{
	public:
		AssertException(StringView exp, const std::source_location& src = std::source_location::current());
		AssertException(StringView exp, StringView msg, const std::source_location& src = std::source_location::current());
	};
}