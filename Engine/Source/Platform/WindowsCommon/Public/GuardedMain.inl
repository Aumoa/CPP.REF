// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "WindowsMinimal.h"
#include "WindowsStackTrace.h"
#include "Misc/CommandLine.h"
#include "Diagnostics/LogModule.h"
#include <iostream>

static int32 GReturn = 0;

DWORD CALLBACK ReportCrash(DWORD ExceptionCode, LPEXCEPTION_POINTERS lpException)
{
	WindowsStackTrace StackTrace(lpException);
	GReturn = (int32)ExceptionCode;

	WCHAR* Buf = nullptr;
	std::wstring ScopedBuf;
	FormatMessageW(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ALLOCATE_BUFFER, GetModuleHandleW(L"ntdll.dll"), ExceptionCode, 0, (WCHAR*)&Buf, 0, nullptr);
	if (Buf)
	{
		ScopedBuf = Buf;
		LocalFree(Buf);
	}
	else
	{
		ScopedBuf = L"<Unknown>";
	}

	SE_LOG(LogWindowsCommon, Error, L"Unhandled exception caught!");
	SE_LOG(LogWindowsCommon, Error, L"Code 0x{:0>8X} - {}", ExceptionCode, ScopedBuf);

	for (auto& Callstack : StackTrace.GetCallstacks())
	{
		std::wstring Callstack_line = std::format(L"{:>2}  {}!{} [{}]", Callstack.FrameNumber, Callstack.ModuleName, Callstack.FunctionName, Callstack.SourceLocation);
		SE_LOG(LogWindowsCommon, Error, Callstack_line);
		std::wcerr << Callstack_line << std::endl;
	}

#if DO_CHECK
	return EXCEPTION_CONTINUE_SEARCH;
#else
	return EXCEPTION_EXECUTE_HANDLER;
#endif
}

int32 GuardedMain(std::span<std::wstring> Argv)
{
	// std::chrono::zoned_time is causes memory leak logs.
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#if !DO_CHECK
	__try
	{
#endif
		GReturn = TApplicationClass::GuardedMain(Argv);
#if !DO_CHECK
	}
	__except (ReportCrash(GetExceptionCode(), GetExceptionInformation()))
	{
		LogModule::Get()->Shutdown();
	}
#endif

	return GReturn;
}

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
	setlocale(LC_ALL, "");

	std::vector<std::wstring> Argv(argc);
	for (size_t i = 0; i < Argv.size(); ++i)
	{
		Argv[i] = argv[i];
	}

	return GuardedMain(Argv);
}

int wWinMain(HINSTANCE, HINSTANCE, wchar_t* argv, int)
{
	std::vector<std::wstring> Argv = String::Split((std::wstring)argv, L" ", true, true);
	return GuardedMain(Argv);
}