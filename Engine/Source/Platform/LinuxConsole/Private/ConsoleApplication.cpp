// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsoleApplication.h"
#include "LogLinuxCommon.h"
#include "ConsoleModule.h"
#include "Misc/PlatformModule.h"
#include "Misc/CommandLine.h"
#include "Diagnostics/LogModule.h"
#include "Threading/Thread.h"
#include "GC/SharedPtr.h"
#include <chrono>

using namespace libty;

extern "C"
{
	Console::SConsoleModule* LoadConsoleModule();
}

int32 SConsoleApplication::GuardedMain(std::span<const std::wstring> Argv)
{
	using namespace std;

	int32 ReturnCode;
	auto Logger = std::make_unique<LogModule>(String::AsUnicode(SE_APPLICATION));
	Logger->RunTask();

	GC.Init();

	{
		auto CommandArgs = CommandLine(Argv);
		SharedPtr ConsoleModule = LoadConsoleModule();
		if (!ConsoleModule.IsValid())
		{
			throw fatal_exception("LoadConsoleModule function return nullptr.");
		}

		Thread* MainThread = Thread::GetCurrentThread();

		MainThread->SetFriendlyName(L"[Main Thread]");
		ReturnCode = ConsoleModule->Main(CommandArgs);
		SE_LOG(LogLinuxCommon, Verbose, L"Application will shutting down with return code: {}.", ReturnCode);
	}

	GC.Collect(true);
	GC.Shutdown(true);

	Logger->Shutdown();
	return ReturnCode;
}