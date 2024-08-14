// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Platform/Linux/LinuxPlatformProcess.h"

#if PLATFORM_LINUX

#include "System/Console.h"
#include "Diagnostics/StackTrace.h"
#include <signal.h>
#include <execinfo.h>

bool LinuxPlatformProcess::IsDebuggerPresent() noexcept
{
	return false;
}

void LinuxPlatformProcess::OutputDebugString(String InMessage) noexcept
{
}

static void Backtrace(std::vector<StackFrame>& OutStackframes, void* lpExceptionPointer)
{
	OutStackframes.clear();

	void* Buffer[128];
	size_t Backtraces = backtrace(Buffer, 128);

	if (lpExceptionPointer)
	{
		Buffer[1] = reinterpret_cast<void*>(reinterpret_cast<sigcontext*>(lpExceptionPointer)->rip);
	}

	char** Messages = backtrace_symbols(Buffer, Backtraces);
	for (size_t i = 1; i < Backtraces; ++i)
	{
		OutStackframes.emplace_back(StackFrame
		{
			.Description = String(Messages[i])
		});
	}

	free(Messages);
}

void LinuxPlatformProcess::StacktraceCurrent(std::vector<StackFrame>& OutStackframes) noexcept
{
	Backtrace(OutStackframes, nullptr);
}

void LinuxPlatformProcess::StacktraceFromThread(void* Handle, std::vector<StackFrame>& OutStackframes) noexcept
{
	OutStackframes.clear();
}

void LinuxPlatformProcess::StacktraceFromException(void* lpExceptionPointer, std::vector<StackFrame>& OutStackframes) noexcept
{
	Backtrace(OutStackframes, lpExceptionPointer);
}

extern "C" void signal_handler(int sig)
{
	Console::Error.WriteLine(StackTrace::Current().ToString());
	exit(sig);
}

void LinuxPlatformProcess::SetupStacktraceSignals() noexcept
{
	signal(SIGSEGV, signal_handler);
	signal(SIGTRAP, signal_handler);
}

#endif