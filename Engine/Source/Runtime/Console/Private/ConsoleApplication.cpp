// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsoleApplication.h"
#include "ConsoleModule.h"
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