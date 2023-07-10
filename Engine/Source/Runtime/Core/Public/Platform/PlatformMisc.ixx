// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.Platform:PlatformMisc;

export import Core.Std;
export import Core.System;

export class String;
export class IOCompletionOverlapped;

export class CORE_API PlatformMisc : public StaticClass
{
public:
	static void InitializeSpinlock(size_t& Lock) noexcept;
	static void DestroySpinlock(size_t& Lock) noexcept;
	static void AcquireSpinlockExclusive(size_t& Lock) noexcept;
	static void ReleaseSpinlockExclusive(size_t& Lock) noexcept;
	static bool TryAcquireSpinlockExclusive(size_t& Lock) noexcept;
	static void AcquireSpinlockShared(size_t& Lock) noexcept;
	static void ReleaseSpinlockShared(size_t& Lock) noexcept;
	static bool TryAcquireSpinlockShared(size_t& Lock) noexcept;

public:
	static void InitializeConditionVariable(size_t& Cv) noexcept;
	static void DestroyConditionVariable(size_t& Cv) noexcept;
	static void NotifyOneConditionVariable(size_t& Cv) noexcept;
	static void NotifyAllConditionVariable(size_t& Cv) noexcept;
	static bool SleepConditionVariable(size_t& Cv, size_t& Lock, uint32 Delay, bool bShared) noexcept;

public:
	static bool SetThreadName(String name) noexcept;
	static String GetThreadName() noexcept;
	static size_t GetCurrentThreadId() noexcept;

public:
	static void Debugbreak() noexcept;
	inline static void YieldProcessor() noexcept
	{
#if PLATFORM_WINDOWS
		_mm_pause();
#else
		std::this_thread::yield();
#endif
	}
	static void OutputDebugString(String message) noexcept;
	static bool IsDebuggerPresent() noexcept;
	[[noreturn]] static void Abort() noexcept;
	template<class T>
	static void UnreferencedParameter(T&&) noexcept
	{
	}

public:
	static void UuidCreate(void* block128) noexcept;
	static String FormatSystemCode(int32 systemCode);
	static int32 GetLastError() noexcept;
	static String GetSystemPath();

public:
	static std::wstring FromCodepage(std::string_view Str, int32 Codepage = 0) noexcept;
	static std::string AsCodepage(std::wstring_view Str, int32 Codepage = 0) noexcept;
};