// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "WindowsMinimal.h"
#include <iostream>

static int32 GReturn = 0;

LONG CALLBACK ReportCrash(LPEXCEPTION_POINTERS lpException)
{
	using namespace ::libty;

	DWORD ExceptionCode = lpException->ExceptionRecord->ExceptionCode;
	Stacktrace StackTrace = Stacktrace::CaptureException(lpException);
	GReturn = (int32)ExceptionCode;

	if (FatalException* fe; ExceptionCode == 0xE06D7363 && (fe = FatalException::AvailableException()))
	{
		SE_LOG(LogWindowsCommon, Error, L"Unhandled C++ exception caught!\n===== BEGIN OF EXCEPTION TRACE =====\n      {}\n===== END OF EXCEPTION TRACE =====", fe->ToString());
	}
	else
	{
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

		SE_LOG(LogWindowsCommon, Error, L"Unhandled SEH exception caught! Code 0x{:0>8X} - {}\n===== BEGIN OF STACKTRACE =====\n{}\n===== END OF STACKTRACE =====", ExceptionCode, ScopedBuf, StackTrace.Trace());
	}

	if (auto* logModule = LogModule::Get())
	{
		logModule->FlushAsync().Wait();
	}

#if DO_CHECK
	return EXCEPTION_CONTINUE_SEARCH;
#else
	return EXCEPTION_EXECUTE_HANDLER;
#endif
}

inline void ShutdownLogger()
{
	using namespace ::libty;
	if (auto* logModule = LogModule::Get())
	{
		Task<> task = logModule->StopAsync();
		task.Wait();
	}
}

int32 GuardedMain(std::span<std::wstring> Argv)
{
	SetUnhandledExceptionFilter(ReportCrash);
	__try
	{
		GReturn = TApplicationClass::GuardedMain(Argv);
	}
	__except (ReportCrash(GetExceptionInformation()))
	{
	}

	ShutdownLogger();
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