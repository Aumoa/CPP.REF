// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "LogSystem.h"
#include "LogCategory.h"
#include "LogVerbosity.h"
#include "Utilities/StringUtils.h"

using namespace std;

LogSystem::fatal_exception::fatal_exception(const std::string& message) : exception(message.c_str())
{
}

void LogSystem::InternalLog(LogCategory& category, ELogVerbosity logVerbosity, wstring& message)
{
	category.OnLog(logVerbosity, message);

	if (logVerbosity == ELogVerbosity::Fatal)
	{
		throw fatal_exception(StringUtils::AsMultibyte(message));
	}
}