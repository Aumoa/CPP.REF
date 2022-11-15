// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/SocketException.h"

SocketException::SocketException(int32 systemCode) noexcept
	: Exception(FormatMessage(systemCode))
{
}

SocketException::SocketException(int32 systemCode, String message) noexcept
	: Exception(FormatMessage(systemCode, message))
{
}

String SocketException::FormatMessage(int32 systemCode, String message) noexcept
{
	return String::Format(TEXT("{}({}: {})"), message, systemCode, PlatformMisc::FormatSystemCode(systemCode).Trim());
}