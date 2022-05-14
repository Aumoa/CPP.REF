// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsoleApplication.h"
#include "ConsoleModule.h"
#include "InternalConsole.h"
#include <csignal>

using namespace ::libty;

static SConsoleApplication* sApp;
static std::stop_source sCancellationTokenSource;

SConsoleApplication::SigintEvent SConsoleApplication::Sigint;

int32 SConsoleApplication::GuardedMain(std::span<const String> Argv)
{
	signal(SIGINT, _Handle_sigint);

	int32 exitCode = 0;
	auto logger = std::make_unique<LogModule>(SE_APPLICATION);
	logger->Logged.AddRaw(&_Console_logged);
	logger->StartAsync(sCancellationTokenSource.get_token()).Wait();

	GC->Init();
	auto commandLine = CommandLine(Argv);

	String moduleName;
	if (!commandLine.TryGetValue(TEXT("ConsoleDll"), moduleName))
	{
		moduleName = SE_APPLICATION_TARGET;
	}

	auto module = std::make_unique<PlatformModule>(moduleName);
	auto loader = module->GetFunctionPointer<SConsoleModule*()>(TEXT("LoadConsoleModule"));
	if (loader == nullptr)
	{
		throw Exception(String::Format(TEXT("Cannot found 'LoadConsoleModule' function from '{0}' module. See 'DEFINE_CONSOLE_MODULE' macros in 'ConsoleModule.h', and declare your module in your code."), moduleName));
	}

	{
		SharedPtr cModule = loader();

		if (!cModule.IsValid())
		{
			throw Exception(TEXT("'LoadConsoleModule' function return nullptr.'"));
		}

		Thread::GetCurrentThread()->SetFriendlyName(TEXT("[Main Thread]"));
		exitCode = cModule->Main(commandLine);
	}

	GC->Collect(true);
	GC->Shutdown(true);

	logger->StopAsync(sCancellationTokenSource.get_token()).Wait();
	return exitCode;
}

void SConsoleApplication::AllocConsole()
{
#if PLATFORM_WINDOWS

#endif
}

void SConsoleApplication::FreeConsole()
{
}

void SConsoleApplication::_Handle_sigint(int)
{
	sCancellationTokenSource.request_stop();
	Sigint.Broadcast();
}

void SConsoleApplication::_Console_logged(const LogEntry& entry)
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
	
	if (entry.ThreadName)
	{
		trd = String::Format(TEXT("[ThreadId {}]"), entry.ThreadId);
	}
	else
	{
		trd = entry.ThreadName;
	}

	InternalConsole::Write(ConsolePin::Null(), String::Format(TEXT("{0}{1}</color>: {2}: {0}{3}</color>\n"),
		ccode,
		verb,
		trd,
		entry.Category->GetName()
	));
	InternalConsole::Write(ConsolePin::Null(), String::Format(L"      {0}\n", String::ReplaceAll(entry.Message, TEXT("<"), TEXT("<<"))));
}