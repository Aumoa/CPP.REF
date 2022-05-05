// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "WindowsMinimal.h"
#include <iostream>

static int32 GReturn = 0;

DWORD CALLBACK ReportCrash(DWORD ExceptionCode, LPEXCEPTION_POINTERS lpException)
{
	using namespace ::libty;

	Stacktrace StackTrace = Stacktrace::CaptureException(lpException);
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
		ScopedBuf = TEXT("<Unknown>");
	}

	std::vector<String> frames;
	for (auto& Callstack : StackTrace.GetStackframes())
	{
		frames.emplace_back(String::Format(TEXT("   at {}!{} in {}{}"),
			Callstack.ModuleName,
			Callstack.GetCleanedFunctionName(),
			Callstack.GetCleanedFileName(),
			Callstack.FileName.empty() ? TEXT("") : String::Format(TEXT("({})"), Callstack.Line)
		));
	}

	String callstack = String::Join(TEXT("\n"), frames);
	SE_LOG(LogWindowsCommon, Error, L"Unhandled exception caught! Code 0x{:0>8X} - {}\n===== BEGIN OF STACKTRACE =====\n{}\n===== END OF STACKTRACE =====", ExceptionCode, ScopedBuf, callstack);

	if (auto* logModule = LogModule::Get())
	{
		Task<> task = logModule->StopAsync();
		task.Wait();
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

	__try
	{
		GReturn = TApplicationClass::GuardedMain(Argv);
	}
	__except (ReportCrash(GetExceptionCode(), GetExceptionInformation()))
	{
	}

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
	std::vector<std::wstring> Argv = ::libty::String::Split((std::wstring)argv, L" ", true, true);
	return GuardedMain(Argv);
}