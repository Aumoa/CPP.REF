// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogVerbosity.h"
#include "Misc/StringUtils.h"

LogSystem::fatal_exception::fatal_exception(const std::string& message) : exception(message.c_str())
{
}

void LogSystem::InternalLog(SLogCategory& category, ELogVerbosity logVerbosity, std::wstring& message)
{
	category.OnLog(logVerbosity, message);

	if (logVerbosity == ELogVerbosity::Fatal)
	{
		throw fatal_exception(StringUtils::AsMultibyte(message));
	}
}