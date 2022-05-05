// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/StringView.h"
#include <exception>
#include <string>
#include <string_view>
#include <source_location>

namespace libty::inline Core
{
	class CORE_API FatalException : public std::exception
	{
		String _message;
		std::source_location _src;
		String _what;
		std::exception_ptr _innerException;

		std::string _what_mb;

	public:
		FatalException(StringView message, std::exception_ptr innerException = {}, const std::source_location & src = std::source_location::current());

		virtual const char* what() const noexcept override;

		virtual const std::source_location& GetSource() const noexcept;
		virtual StringView GetMessage() const noexcept;
		virtual std::exception_ptr GetInnerException() const noexcept;
	};
}
