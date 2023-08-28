// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Platform/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include "Platform/Windows/WindowsPlatformProcess.h"
#include "System/Console.h"
#include "Diagnostics/StackFrame.h"
#include "Diagnostics/StackTrace.h"
#include "WindowsCore.h"
#include "WindowsStackTrace.h"

bool WindowsPlatformProcess::IsDebuggerPresent() noexcept
{
	return ::IsDebuggerPresent() == TRUE;
}

void WindowsPlatformProcess::OutputDebugString(String InMessage) noexcept
{
	::OutputDebugStringW(InMessage.c_str());
}

void WindowsPlatformProcess::StacktraceCurrent(std::vector<StackFrame>& OutStackframes) noexcept
{
	HANDLE hThread = GetCurrentThread();

	CONTEXT Context;
	RtlCaptureContext(&Context);

	OutStackframes = Capture(hThread, &Context);
}

void WindowsPlatformProcess::StacktraceFromThread(void* Handle, std::vector<StackFrame>& OutStackframes) noexcept
{
	HANDLE hThread = (HANDLE)Handle;
	const bool bSameThread = GetThreadId(hThread) == GetThreadId(GetCurrentThread());
	if (bSameThread)
	{
		StacktraceCurrent(OutStackframes);
		return;
	}

	SuspendThread(hThread);

	CONTEXT Context;
	GetThreadContext(hThread, &Context);

	OutStackframes = Capture(hThread, &Context);

	ResumeThread(hThread);
}

void WindowsPlatformProcess::StacktraceFromException(void* lpExceptionPointers, std::vector<StackFrame>& OutStackframes) noexcept
{
	OutStackframes = Capture(GetCurrentThread(), ((LPEXCEPTION_POINTERS)lpExceptionPointers)->ContextRecord);
}

extern "C" LONG CALLBACK UnhandledExceptionTrace(LPEXCEPTION_POINTERS lpExceptionPointer)
{
	Console::Error.WriteLine(StackTrace::FromException(lpExceptionPointer).ToString());
	return EXCEPTION_CONTINUE_SEARCH;
}

void WindowsPlatformProcess::SetupStacktraceSignals() noexcept
{
	SetUnhandledExceptionFilter(UnhandledExceptionTrace);
}

#endif