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

	if (Exception* e; ExceptionCode == 0xE06D7363 && (e = Exception::AvailableException()))
	{
		SE_LOG(LogWindowsCommon, Error, TEXT("Unhandled C++ exception caught!\n===== BEGIN OF EXCEPTION TRACE =====\n      {}\n===== END OF EXCEPTION TRACE ====="), e->ToString());
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

		SE_LOG(LogWindowsCommon, Error, TEXT("Unhandled SEH exception caught! Code 0x{:0>8X} - {}\n===== BEGIN OF STACKTRACE =====\n{}\n===== END OF STACKTRACE ====="), ExceptionCode, ScopedBuf, StackTrace.Trace());
	}

	if (auto* logModule = LogModule::Get())
	{
		logModule->FlushAsync().Wait();
	}

	return EXCEPTION_EXECUTE_HANDLER;
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

int32 GuardedMain(std::span<::libty::String> Argv)
{
#if !DO_CHECK
	__try
	{
#endif
		GReturn = TApplicationClass::GuardedMain(Argv);
#if !DO_CHECK
	}
	__except (ReportCrash(GetExceptionInformation()))
	{
	}
#endif

	ShutdownLogger();
	return GReturn;
}

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
	using namespace ::libty;

	setlocale(LC_ALL, "");

	std::vector<String> Argv(argc);
	for (size_t i = 0; i < Argv.size(); ++i)
	{
		Argv[i] = String::FromLiteral(argv[i]);
	}

	return GuardedMain(Argv);
}

int wWinMain(HINSTANCE, HINSTANCE, wchar_t* argv, int)
{
	using namespace ::libty;

	std::vector<String> Argv = ::libty::String(argv).Split(TEXT(" "), ::libty::EStringSplitOptions::RemoveEmptyEntries | ::libty::EStringSplitOptions::TrimEntries);
	return GuardedMain(Argv);
}