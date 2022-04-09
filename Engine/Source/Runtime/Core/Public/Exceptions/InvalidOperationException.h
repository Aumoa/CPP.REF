// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FatalException.h"

namespace libty::inline Core::inline Exceptions
{
	class CORE_API InvalidOperationException : public FatalException
	{
	public:
		InvalidOperationException(std::exception_ptr innerException = nullptr, std::source_location src = std::source_location::current());
		InvalidOperationException(std::string_view message, std::exception_ptr innerException = nullptr, std::source_location src = std::source_location::current());
	};
}