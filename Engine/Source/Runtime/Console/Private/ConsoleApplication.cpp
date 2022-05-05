// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsoleApplication.h"
#include "ConsoleModule.h"
#include "InternalConsole.h"
#include <csignal>

using namespace ::libty;

static SConsoleApplication* sApp;
static std::stop_source sCancellationTokenSource;

SConsoleApplication::SigintEvent SConsoleApplication::Sigint;

int32 SConsoleApplication::GuardedMain(std::span<const std::wstring> Argv)
{
	signal(SIGINT, _Handle_sigint);

	int32 exitCode = 0;
	auto logger = std::make_unique<LogModule>(String::AsUnicode(SE_APPLICATION));
	logger->Logged.AddRaw(&_Console_logged);
	logger->StartAsync(sCancellationTokenSource.get_token()).Wait();

	GC->Init();
	auto commandLine = CommandLine(Argv);

	std::wstring moduleName;
	if (!commandLine.TryGetValue(L"ConsoleDll", moduleName))
	{
		moduleName = String::AsUnicode(SE_APPLICATION_TARGET);
	}

	auto module = std::make_unique<PlatformModule>(moduleName);
	auto loader = module->GetFunctionPointer<SConsoleModule*()>("LoadConsoleModule");
	if (loader == nullptr)
	{
		throw FatalException(String::Format("Cannot found 'LoadConsoleModule' function from '{0}' module. See 'DEFINE_CONSOLE_MODULE' macros in 'ConsoleModule.h', and declare your module in your code.", String::AsMultibyte(moduleName)));
	}

	{
		SharedPtr cModule = loader();

		if (!cModule.IsValid())
		{
			throw FatalException("'LoadConsoleModule' function return nullptr.'");
		}

		Thread::GetCurrentThread()->SetFriendlyName(L"[Main Thread]");
		exitCode = cModule->Main(commandLine);
	}

	GC->Collect(true);
	GC->Shutdown(true);

	logger->StopAsync(sCancellationTokenSource.get_token()).Wait();
	return exitCode;
}

void SConsoleApplication::_Handle_sigint(int)
{
	sCancellationTokenSource.request_stop();
	Sigint.Broadcast();
}

void SConsoleApplication::_Console_logged(const LogEntry& entry)
{
	std::wstring ccode;
	std::wstring verb;

	switch (entry.Verbosity)
	{
	case ELogVerbosity::Error:
		verb = L"erro";
		ccode = L"<color=Red>";
		break;
	case ELogVerbosity::Warning:
		verb = L"warn";
		ccode = L"<color=Yellow>";
		break;
	case ELogVerbosity::Info:
		verb = L"info";
		ccode = L"<color=Green>";
		break;
	case ELogVerbosity::Verbose:
		verb = L"verb";
		ccode = L"<color=White>";
		break;
	}

	std::wostringstream woss;
	if (entry.ThreadName.empty())
	{
		woss << L"[ThreadId " << entry.ThreadId << L']';
	}
	else
	{
		woss << entry.ThreadName;
	}

	InternalConsole::Write(ConsolePin::Null(), String::Format(L"{0}{1}</color>: {2}: {0}{3}</color>\n", ccode, verb, std::move(woss).str(), entry.Category->GetName()));
	InternalConsole::Write(ConsolePin::Null(), String::Format(L"      {0}\n", String::ReplaceAll(entry.Message, TEXT("<"), TEXT("<<"))));
}