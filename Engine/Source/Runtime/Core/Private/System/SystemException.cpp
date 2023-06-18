// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core.System;
import Core.Platform;

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