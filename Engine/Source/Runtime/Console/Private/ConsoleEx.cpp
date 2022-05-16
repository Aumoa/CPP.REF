// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsoleEx.h"
#include "InternalConsole.h"
#include "ConsolePin.h"

using namespace ::libty;

void ConsoleEx::Write(ConsolePin pin, String message)
{
	LogModule::Get()->EnqueueLogAction([pin, message]() mutable
	{
		InternalConsole::Write(pin, message);
	});
}

void ConsoleEx::Clear(ConsolePin pin)
{
	LogModule::Get()->EnqueueLogAction([pin]() mutable
	{
		InternalConsole::Clear(pin, true);
	});
}

void ConsoleEx::EnableLogToConsole(bool enabled)
{
	static DelegateHandle sLogged;

	if (auto logger = LogModule::Get())
	{
		if (enabled)
		{
			check(!sLogged.IsValid());
			sLogged = logger->Logged.AddRaw(&_Console_logged);
		}
		else
		{
			sLogged.Reset();
		}
	}
}

void ConsoleEx::_Console_logged(const LogEntry& entry)
{
	String ccode;
	String verb;

	switch (entry.Verbosity)
	{
	case ELogVerbosity::Error:
		verb = TEXT("erro");
		ccode = TEXT("<color=Red>");
		break;
	case ELogVerbosity::Warning:
		verb = TEXT("warn");
		ccode = TEXT("<color=Yellow>");
		break;
	case ELogVerbosity::Info:
		verb = TEXT("info");
		ccode = TEXT("<color=Green>");
		break;
	case ELogVerbosity::Verbose:
		verb = TEXT("verb");
		ccode = TEXT("<color=White>");
		break;
	}

	String trd;

	if (!entry.ThreadName)
	{
		std::wostringstream woss;
		woss << entry.ThreadId;
		trd = String::Format(TEXT("[ThreadId {}]"), woss.str());
	}
	else
	{
		trd = entry.ThreadName;
	}

	InternalConsole::Write(ConsolePin::Null(), String::Format(TEXT("{0}{1}</color>: {2}: {0}{3}</color>\n      {4}\n"),
		ccode,
		verb,
		trd,
		entry.Category->GetName(),
		entry.Message.Replace(TEXT("<"), TEXT("<<"))
	));
}