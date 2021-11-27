// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "WindowsMinimal.h"
#include "WindowsStackTrace.h"
#include "Misc/CommandLine.h"

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
		SE_LOG(LogWindowsCommon, Error, L"{:>2}  {}!{} [{}]", Callstack.FrameNumber, Callstack.ModuleName, Callstack.FunctionName, Callstack.SourceLocation);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

int32 GuardedMain(std::span<std::wstring_view> Argv)
{
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

	std::vector<std::wstring_view> Argv(argc);
	for (size_t i = 0; i < Argv.size(); ++i)
	{
		Argv[i] = argv[i];
	}

	return GuardedMain(Argv);
}