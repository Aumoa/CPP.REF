// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsoleApplication.h"
#include "ConsoleModule.h"
#include "InternalConsole.h"
#include "ConsoleEx.h"
#include <csignal>

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT

#define NOMINMAX

#include <Windows.h>

#pragma pop_macro("TEXT")

#endif

using namespace ::libty;

static SConsoleApplication* sApp;
static std::stop_source sCancellationTokenSource;

SConsoleApplication::SigintEvent SConsoleApplication::Sigint;

int32 SConsoleApplication::GuardedMain(std::span<const String> Argv)
{
	signal(SIGINT, _Handle_sigint);

	int32 exitCode = 0;
	auto logger = std::make_unique<LogModule>(SE_APPLICATION);
	ConsoleEx::EnableLogToConsole(true);
	logger->StartAsync(sCancellationTokenSource.get_token()).Wait();

	GC->Init();
	auto commandLine = CommandLine(Argv);

	String moduleName;
	if (!commandLine.TryGetValue(TEXT("ConsoleDll"), 0, moduleName))
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

	ConsoleEx::EnableLogToConsole(false);
	logger->StopAsync(sCancellationTokenSource.get_token()).Wait();

	return exitCode;
}

void SConsoleApplication::_Handle_sigint(int)
{
	sCancellationTokenSource.request_stop();
	Sigint.Broadcast();
}