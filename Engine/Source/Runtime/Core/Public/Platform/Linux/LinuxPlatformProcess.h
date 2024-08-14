// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_LINUX

#include "Platform/Unix/UnixPlatformProcess.h"
#include "Diagnostics/StackFrame.h"
#include "System/String.h"
#include <vector>

struct LinuxPlatformProcess : public UnixPlatformProcess
{
	static bool IsDebuggerPresent() noexcept;
	static void OutputDebugString(String InMessage) noexcept;

	static void StacktraceCurrent(std::vector<StackFrame>& OutStackframes) noexcept;
	static void StacktraceFromThread(void* Handle, std::vector<StackFrame>& OutStackframes) noexcept;
	static void StacktraceFromException(void* lpExceptionPointer, std::vector<StackFrame>& OutStackframes) noexcept;

	static void SetupStacktraceSignals() noexcept;
};

using PlatformProcess = LinuxPlatformProcess;

#endif