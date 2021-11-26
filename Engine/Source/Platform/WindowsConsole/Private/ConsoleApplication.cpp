// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ConsoleApplication.h"
#include "LogWindowsConsole.h"
#include "ConsoleModule.h"
#include "PlatformMisc/PlatformModule.h"
#include "Misc/CommandLine.h"

int32* GPointer;

int32 SConsoleApplication::GuardedMain(std::span<const std::wstring_view> Argv)
{
	using namespace std;
	
	SCommandLine CommandArgs = Argv;

	size_t ConsoleModuleIdx = CommandArgs.GetArgument(L"--ConsoleDll");
	if (ConsoleModuleIdx == -1)
	{
		SE_LOG(LogWindowsConsole, Fatal, L"ConsoleModule does not specified.");
		return -1;
	}

	std::optional<std::wstring_view> ModuleName = CommandArgs.GetArgument(ConsoleModuleIdx + 1);
	if (!ModuleName)
	{
		SE_LOG(LogWindowsConsole, Fatal, L"ConsoleModule does not specified.");
		return -1;
	}

	SPlatformModule Module(*ModuleName);
	auto Loader = Module.GetFunctionPointer<SConsoleModule* (SObject*)>("LoadConsoleModule");
	if (!Loader)
	{
		SE_LOG(LogWindowsConsole, Fatal, L"Cannot found 'LoadConsoleModule' function from Module: {}.", *ModuleName);
		return -1;
	}

	SConsoleModule* ConsoleModule = Loader(&Module);
	if (ConsoleModule == nullptr)
	{
		SE_LOG(LogWindowsConsole, Fatal, L"LoadConsoleModule function return nullptr.");
		return -1;
	}

	return ConsoleModule->Main(CommandArgs);
}