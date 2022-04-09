// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <exception>
#include <string>
#include <string_view>
#include <source_location>

namespace libty::inline Core::inline Exceptions
{
	class CORE_API FatalException : public std::exception
	{
		std::string _message;
		std::source_location _src;
		std::string _what;
		std::exception_ptr _innerException;

	public:
		FatalException(std::string_view message, std::exception_ptr innerException = {}, const std::source_location & src = std::source_location::current());

		virtual const char* what() const noexcept override;

		virtual const std::source_location& GetSource() const noexcept;
		virtual std::string_view GetMessage() const noexcept;
		virtual std::exception_ptr GetInnerException() const noexcept;
	};
}
