// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/PlatformMacros.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/Windows/WindowsPlatformProcess.h"
#include "Platform/PlatformCommon.h"
#include "Console.h"
#include "Path.h"
#include "Diagnostics/StackFrame.h"
#include "Diagnostics/StackTrace.h"
#include "WindowsStackTrace.h"

namespace Ayla
{
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

	String WindowsPlatformProcess::FindEngineDirectory()
	{
		// Find 'Core.dll' library.
		HMODULE hModule = GetModuleHandleW(TEXT("Core.dll").c_str());
		if (hModule == nullptr)
		{
			return String::GetEmpty();
		}

		TCHAR Buf[1024];
		DWORD Len = GetModuleFileNameW(hModule, Buf, 1024);
		if (Len == 0)
		{
			return String::GetEmpty();
		}

		// Get engine directory from relative path of 'Core.dll'.
		String FileName = String(Buf, Len);
		// Engine/Binaries/{Platform}/{Configuration}/
		String BinariesDirectory = Path::GetDirectoryName(FileName);
		// Engine/
		String EngineDirectory = Path::Combine(BinariesDirectory, TEXT(".."), TEXT(".."), TEXT(".."));
		EngineDirectory = Path::GetFullPath(EngineDirectory);

		return EngineDirectory;
	}

	void* WindowsPlatformProcess::AllocateCurrentThreadHandle() noexcept
	{
		HANDLE hCurrentProcess = GetCurrentProcess();
		HANDLE hCurrentThread = GetCurrentThread();
		DuplicateHandle(hCurrentProcess, hCurrentThread, hCurrentProcess, &hCurrentThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
		return hCurrentThread;
	}

	void WindowsPlatformProcess::DestroyCurrentThreadHandle(void* InHandle) noexcept
	{
		CloseHandle((HANDLE)InHandle);
	}

	void WindowsPlatformProcess::SetThreadDescription(void* InHandle, String InDescription) noexcept
	{
		::SetThreadDescription((HANDLE)InHandle, (LPCWSTR)InDescription.c_str());
	}

	void* WindowsPlatformProcess::CreateProcess(const ProcessStartInfo& InStartInfo) noexcept
	{
		STARTUPINFOW StartupInfo = {};
		PROCESS_INFORMATION ProcessInfo = {};

		BOOL bStatus = ::CreateProcessW(
			InStartInfo.FileName.c_str(),
			(LPWSTR)InStartInfo.Arguments.c_str(),
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&StartupInfo,
			&ProcessInfo
		);
		check(bStatus);

		if (bStatus)
		{
			CloseHandle(ProcessInfo.hThread);
			ProcessInfo.hThread = NULL;
		}

		return ProcessInfo.hProcess;
	}

	void WindowsPlatformProcess::CloseProcessHandle(void* InHandle) noexcept
	{
		BOOL bStatus = CloseHandle((HANDLE)InHandle);
		check(bStatus);
	}

	bool WindowsPlatformProcess::SetEnvironmentVariable(String InName, String InValue) noexcept
	{
		return ::SetEnvironmentVariableW(InName.c_str(), InValue.c_str()) == TRUE;
	}

	String WindowsPlatformProcess::GetEnvironmentVariable(String InName) noexcept
	{
		static thread_local WCHAR Buf[32767];
		DWORD Len = ::GetEnvironmentVariableW(InName.c_str(), Buf, (DWORD)AE_ARRAYSIZE(Buf));
		if (Len == 0)
		{
			return String::GetEmpty();
		}
		return String::FromLiteral(std::wstring_view(Buf, (size_t)Len));
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif