// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include "Platform/Generic/GenericPlatformProcess.h"
#include "System/String.h"
#include <vector>

struct StackFrame;

struct CORE_API WindowsPlatformProcess : public GenericPlatformProcess
{
	static bool IsDebuggerPresent() noexcept;
	static void OutputDebugString(String InMessage) noexcept;

	static void StacktraceCurrent(std::vector<StackFrame>& OutStackframes) noexcept;
	static void StacktraceFromThread(void* Handle, std::vector<StackFrame>& OutStackframes) noexcept;
	static void StacktraceFromException(void* lpExceptionPointer, std::vector<StackFrame>& OutStackframes) noexcept;

	static void SetupStacktraceSignals() noexcept;

	static void* AllocateCurrentThreadHandle() noexcept;
	static void DestroyCurrentThreadHandle(void* InHandle) noexcept;
	static void SetThreadDescription(void* InHandle, String InDescription) noexcept;
};

using PlatformProcess = WindowsPlatformProcess;

#endif