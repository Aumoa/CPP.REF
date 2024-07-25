// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:WindowsPlatformProcess;

export import :Std;
export import :GenericPlatformProcess;
export import :ProcessStartInfo;
export import :String;

#if PLATFORM_WINDOWS

export struct StackFrame;

export class CORE_API WindowsPlatformProcess : public GenericPlatformProcess
{
public:
	static bool IsDebuggerPresent() noexcept;
	static void OutputDebugString(String InMessage) noexcept;

	static void StacktraceCurrent(std::vector<StackFrame>& OutStackframes) noexcept;
	static void StacktraceFromThread(void* Handle, std::vector<StackFrame>& OutStackframes) noexcept;
	static void StacktraceFromException(void* lpExceptionPointer, std::vector<StackFrame>& OutStackframes) noexcept;

	static void SetupStacktraceSignals() noexcept;
	static String FindEngineDirectory();

	static void* AllocateCurrentThreadHandle() noexcept;
	static void DestroyCurrentThreadHandle(void* InHandle) noexcept;
	static void SetThreadDescription(void* InHandle, String InDescription) noexcept;

	static void* CreateProcess(const ProcessStartInfo& InStartInfo) noexcept;
	static void CloseProcessHandle(void* InHandle) noexcept;

	static bool SetEnvironmentVariable(String InName, String InValue) noexcept;
	static String GetEnvironmentVariable(String InName) noexcept;
};

#endif