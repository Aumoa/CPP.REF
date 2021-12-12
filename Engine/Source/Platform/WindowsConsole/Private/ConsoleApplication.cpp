// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ConsoleApplication.h"
#include "LogWindowsConsole.h"
#include "ConsoleModule.h"
#include "PlatformMisc/PlatformModule.h"
#include "Misc/CommandLine.h"
#include "Diagnostics/LogModule.h"

int32 SConsoleApplication::GuardedMain(std::span<const std::wstring> Argv)
{
	using namespace std;
	
	int32 ReturnCode;

	std::unique_ptr<PlatformModule> Module;

	{
		GCRoot CommandArgs = NewObject<SCommandLine>(Argv);

		size_t ConsoleModuleIdx = CommandArgs->GetArgument(L"--ConsoleDll");
		if (ConsoleModuleIdx == -1)
		{
			SE_LOG(LogWindowsConsole, Fatal, L"ConsoleModule does not specified.");
			return -1;
		}

		std::optional<std::wstring_view> ModuleName = CommandArgs->GetArgument(ConsoleModuleIdx + 1);
		if (!ModuleName)
		{
			SE_LOG(LogWindowsConsole, Fatal, L"ConsoleModule does not specified.");
			return -1;
		}

		GCRoot LogModule = NewObject<SLogModule>(*ModuleName);
		LogModule->RunTask();

		Module = std::make_unique<PlatformModule>(*ModuleName);
		auto Loader = Module->GetFunctionPointer<SConsoleModule* ()>("LoadConsoleModule");
		if (!Loader)
		{
			SE_LOG(LogWindowsConsole, Fatal, L"Cannot found 'LoadConsoleModule' function from Module: {}.", *ModuleName);
			return -1;
		}

		GCRoot ConsoleModule = Loader();
		if (!ConsoleModule.IsValid())
		{
			SE_LOG(LogWindowsConsole, Fatal, L"LoadConsoleModule function return nullptr.");
			return -1;
		}

		ReturnCode = ConsoleModule->Main(*CommandArgs.Get());

		SE_LOG(LogWindowsConsole, Verbose, L"Application will shutting down with return code: {}.", ReturnCode);
		LogModule->Shutdown();
	}

	GC().Collect(true);
	return ReturnCode;
}