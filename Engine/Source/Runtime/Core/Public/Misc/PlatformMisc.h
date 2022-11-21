// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "StaticClass.h"
#include "IO/FileAccessMode.h"
#include "IO/FileSharedMode.h"
#include <expected>
#include <functional>
#include <span>

class String;
class IOCompletionOverlapped;

class CORE_API PlatformMisc : public StaticClass
{
public:
	static void InitializeSpinlock(size_t& lock) noexcept;
	static void DestroySpinlock(size_t& lock) noexcept;
	static void AcquireSpinlockExclusive(size_t& lock) noexcept;
	static void ReleaseSpinlockExclusive(size_t& lock) noexcept;
	static bool TryAcquireSpinlockExclusive(size_t& lock) noexcept;
	static void AcquireSpinlockShared(size_t& lock) noexcept;
	static void ReleaseSpinlockShared(size_t& lock) noexcept;
	static bool TryAcquireSpinlockShared(size_t& lock) noexcept;

public:
	static void InitializeConditionVariable(size_t& cv) noexcept;
	static void DestroyConditionVariable(size_t& cv) noexcept;
	static void NotifyOneConditionVariable(size_t& cv) noexcept;
	static void NotifyAllConditionVariable(size_t& cv) noexcept;
	static bool SleepConditionVariable(size_t& cv, size_t& lock, uint32 delay, bool shared) noexcept;

public:
	static bool SetThreadName(String name) noexcept;
	static String GetThreadName() noexcept;
	static size_t GetCurrentThreadId() noexcept;
	static int GuardedMain(std::function<int()> body);

public:
	static void Debugbreak() noexcept;
#if PLATFORM_WINDOWS
	inline static void YieldProcessor() noexcept { _mm_pause(); }
#endif
	static void OutputDebugString(String message) noexcept;

public:
	static void UuidCreate(void* block128) noexcept;
	static String FormatSystemCode(int32 systemCode) noexcept;
	static int32 GetLastError() noexcept;

public:
	static std::wstring FromCodepage(std::string_view s, int32 codepage = 0) noexcept;
	static std::string AsCodepage(std::wstring_view s, int32 codepage = 0) noexcept;
};