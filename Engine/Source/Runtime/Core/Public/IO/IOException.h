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
		IOException(StringView message, const std::error_code& ec, std::exception_ptr innerException = nullptr) noexcept
			: FatalException(message, innerException)
			, _ec(ec)
		{
		}

		IOException(const std::error_code& ec, std::exception_ptr innerException = nullptr) noexcept
			: FatalException(TEXT("Invalid file operation."), innerException)
			, _ec(ec)
		{
		}

		inline const std::error_code& GetErrorCode() const noexcept
		{
			return _ec;
		}
	};
}