// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ConsoleApplication.h"
#include "LogWindowsConsole.h"
#include "ConsoleModule.h"
#include "PlatformMisc/PlatformModule.h"
#include "Misc/CommandLine.h"
#include "Diagnostics/LogModule.h"
#include "Threading/Thread.h"
#include "Threading/DeferredTaskRunner.h"
#include "Console.h"
#include <chrono>

GENERATE_BODY(SConsoleApplication);

int32 SConsoleApplication::GuardedMain(std::span<const std::wstring> Argv)
{
	using namespace std;
	
	int32 ReturnCode;
	std::unique_ptr<PlatformModule> Module;
	auto Logger = std::make_unique<LogModule>(ANSI_TO_WCHAR(SE_APPLICATION));
	Logger->RunTask();

	GC.Init();

	{
		SharedPtr CommandArgs = gcnew SCommandLine(Argv);

		std::optional<std::wstring> ModuleName;
		size_t ConsoleModuleIdx = CommandArgs->GetArgument(L"--ConsoleDll");
		if (ConsoleModuleIdx != -1)
		{
			ModuleName = CommandArgs->GetArgument(ConsoleModuleIdx + 1);
			if (!ModuleName)
			{
				SE_LOG(LogWindowsConsole, Fatal, L"ConsoleModule does not specified.");
				return -1;
			}
		}
		else
		{
			ModuleName = ANSI_TO_WCHAR(SE_APPLICATION_TARGET);
		}

		Module = std::make_unique<PlatformModule>(*ModuleName);
		auto Loader = Module->GetFunctionPointer<SConsoleModule* ()>("LoadConsoleModule");
		if (!Loader)
		{
			SE_LOG(LogWindowsConsole, Fatal, L"Cannot found 'LoadConsoleModule' function from Module: {}.", *ModuleName);
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
			ReturnCode = ConsoleModule->Main(*CommandArgs.Get());

			DeferredTaskRunner::Stop();
			SE_LOG(LogWindowsConsole, Verbose, L"Application will shutting down with return code: {}.", ReturnCode);
		}
	}

	GC.Collect(true);
	GC.Shutdown(true);

	Logger->Shutdown();
	return ReturnCode;
}