// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "SystemException.h"
#include "Platform/PlatformMisc.h"

namespace Ayla
{
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
}