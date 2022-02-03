// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogVerbosity.h"
#include "Misc/StringUtils.h"
#include "Exceptions/Exception.h"

class LogSystem::SFatalException : public SException
{
	GENERATED_BODY(SFatalException)

public:
	SFatalException(std::wstring_view Message) : Super(Message)
	{
	}
};

GENERATE_BODY(LogSystem::SFatalException);

void LogSystem::InternalLog(LogCategory& Category, ELogVerbosity LogVerbosity, std::wstring& Message)
{
	Category.OnLog(LogVerbosity, Message);

	if (LogVerbosity == ELogVerbosity::Fatal)
	{
		throw gcnew SFatalException(Message);
	}
}