// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FatalException.h"

namespace libty::inline Core
{
	class CORE_API AssertException : public FatalException
	{
	public:
		AssertException(std::string_view exp, const std::source_location& src = std::source_location::current());
		AssertException(std::string_view exp, std::string_view msg, const std::source_location& src = std::source_location::current());
		AssertException(std::string_view exp, std::wstring_view msg, const std::source_location& src = std::source_location::current());
	};
}