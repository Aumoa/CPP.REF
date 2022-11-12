// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/SystemException.h"

SystemException::SystemException(int32 systemCode) noexcept
	: Exception(FormatMessage(systemCode))
{
}

SystemException::SystemException(int32 systemCode, String message) noexcept
	: Exception(FormatMessage(systemCode, message))
{
}

String SystemException::FormatMessage(int32 systemCode, String message) noexcept
{
	return String::Format(TEXT("{}({}: {})"), message, systemCode, PlatformMisc::FormatSystemCode(systemCode));
}