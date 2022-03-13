// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ConsoleApplication.h"
#include "LogWindowsConsole.h"
#include "ConsoleModule.h"
#include "Misc/PlatformModule.h"
#include "Misc/CommandLine.h"
#include "Diagnostics/LogModule.h"
#include "Threading/Thread.h"
#include "GC/SharedPtr.h"
#include <chrono>

GENERATE_BODY(SConsoleApplication);

int32 SConsoleApplication::GuardedMain(std::span<const std::wstring> Argv)
{
	using namespace std;
	
	int32 ReturnCode;
	std::unique_ptr<PlatformModule> Module;
	auto Logger = std::make_unique<LogModule>(String::AsUnicode(SE_APPLICATION));
	Logger->RunTask();

	GC.Init();

	{
		auto CommandArgs = CommandLine(Argv);

		std::wstring ModuleName;
		if (!CommandArgs.TryGetValue(L"ConsoleDll", ModuleName))
		{
			ModuleName = String::AsUnicode(SE_APPLICATION_TARGET);
		}

		Module = std::make_unique<PlatformModule>(ModuleName);
		auto Loader = Module->GetFunctionPointer<SConsoleModule* ()>("LoadConsoleModule");
		if (!Loader)
		{
			SE_LOG(LogWindowsConsole, Fatal, L"Cannot found 'LoadConsoleModule' function from Module: {}.", ModuleName);
			return -1;
		}

		{
			SharedPtr ConsoleModule = Loader();
			if (!ConsoleModule.IsValid())
			{
				SE_LOG(LogWindowsConsole, Fatal, L"LoadConsoleModule function return nullptr.");
				return -1;
			}

			Thread* MainThread = Thread::GetCurrentThread();

			MainThread->SetFriendlyName(L"[Main Thread]");
			ReturnCode = ConsoleModule->Main(CommandArgs);
			SE_LOG(LogWindowsConsole, Verbose, L"Application will shutting down with return code: {}.", ReturnCode);
		}
	}

	GC.Collect(true);
	GC.Shutdown(true);

	Logger->Shutdown();
	return ReturnCode;
}