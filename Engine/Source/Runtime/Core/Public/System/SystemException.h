// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/Exception.h"

class CORE_API SystemException : public Exception
{
public:
	SystemException(int32 InSystemCode) noexcept;
	SystemException(const std::error_code& InErrorCode) noexcept : SystemException(InErrorCode.value()) {}
	SystemException(int32 InSystemCode, String InMessage) noexcept;
	SystemException(const std::error_code& InErrorCode, String InMessage) noexcept : SystemException(InErrorCode.value(), InMessage) {}

private:
	static String FormatMessage(int32 InSystemCode, String InMessage = TEXT("")) noexcept;
};