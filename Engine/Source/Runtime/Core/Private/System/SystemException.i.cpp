// Copyright 2020-2024 Aumoa.lib. All right reserved.

#include "System/LanguageSupportMacros.h"

import Core;

SystemException::SystemException(int32 SystemCode) noexcept
	: Exception(FormatMessage(SystemCode))
{
}

SystemException::SystemException(int32 SystemCode, String InMessage) noexcept
	: Exception(FormatMessage(SystemCode, InMessage))
{
}

String SystemException::FormatMessage(int32 SystemCode, String InMessage) noexcept
{
	return String::Format(TEXT("{}({}: {})"), InMessage, SystemCode, PlatformMisc::FormatSystemCode(SystemCode).Trim());
}