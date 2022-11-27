// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Exceptions/Exception.h"

class CORE_API SystemException : public Exception
{
public:
	SystemException(int32 systemCode) noexcept;
	SystemException(const std::error_code& se) noexcept : SystemException(se.value()) {}
	SystemException(int32 systemCode, String message) noexcept;
	SystemException(const std::error_code& se, String message) noexcept : SystemException(se.value(), message) {}

private:
	static String FormatMessage(int32 systemCode, String message = TEXT("")) noexcept;
};