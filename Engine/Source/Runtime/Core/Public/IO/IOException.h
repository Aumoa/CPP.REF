// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exceptions/FatalException.h"
#include <system_error>

namespace libty::inline Core
{
	class IOException : public FatalException
	{
		std::error_code _ec;

	public:
		IOException(const std::string& message, const std::error_code& ec, std::exception_ptr innerException = nullptr, std::source_location src = std::source_location::current()) noexcept
			: FatalException(message, innerException, src)
			, _ec(ec)
		{
		}

		IOException(const std::error_code& ec, std::exception_ptr innerException = nullptr, std::source_location src = std::source_location::current()) noexcept
			: FatalException("Invalid file operation.", innerException, src)
			, _ec(ec)
		{
		}

		inline const std::error_code& GetErrorCode() const noexcept
		{
			return _ec;
		}
	};
}