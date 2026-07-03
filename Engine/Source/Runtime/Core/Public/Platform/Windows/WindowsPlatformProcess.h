// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include "Platform/Generic/GenericPlatformProcess.h"
#include "Platform/ProcessStartInfo.h"
#include "String_.h"

namespace Ayla
{
	struct StackFrame;

	struct CORE_API WindowsPlatformProcess : public GenericPlatformProcess
	{
		struct SnapshotRunner;
		struct SuspendToken;

		static bool IsDebuggerPresent() noexcept;
		static void OutputDebugString(String InMessage) noexcept;

		static String FindExecutableDirectory();
		static String FindEngineDirectory();

		static void* AllocateCurrentThreadHandle() noexcept;
		static void DestroyCurrentThreadHandle(void* InHandle) noexcept;
		static void SetThreadDescription(void* InHandle, String InDescription) noexcept;

		static void* CreateProcess(const ProcessStartInfo& InStartInfo) noexcept;
		static void CloseProcessHandle(void* InHandle) noexcept;

		static bool SetEnvironmentVariable(String InName, String InValue) noexcept;
		static String GetEnvironmentVariable(String InName) noexcept;

		static SuspendToken* SuspendAllThreads() noexcept;
		static void ResumeAllThreads(SuspendToken* token) noexcept;

		static void* LoadLibrary(String fileName) noexcept;
		static void FreeLibrary(void* handle) noexcept;
		static void* GetLibraryFunction(void* handle, String functionName) noexcept;
	};

	using PlatformProcess = WindowsPlatformProcess;
}

#endif
