// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exceptions/Exception.h"
#include <system_error>

class IOException : public Exception
{
	std::error_code _ec;

public:
	IOException(String message, const std::error_code& ec, std::exception_ptr innerException = nullptr) noexcept
		: Exception(message, innerException)
		, _ec(ec)
	{
	}

	IOException(const std::error_code& ec, std::exception_ptr innerException = nullptr) noexcept
		: Exception(String::FromCodepage(ec.message()), innerException)
		, _ec(ec)
	{
	}

	inline const std::error_code& GetErrorCode() const noexcept
	{
		return _ec;
	}
};