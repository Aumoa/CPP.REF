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
	static void YieldProcessor() noexcept;
	static void OutputDebugString(String message) noexcept;

public:
	static void UuidCreate(void* block128) noexcept;
	static String FormatSystemCode(int32 systemCode) noexcept;
	static int32 GetLastError() noexcept;

public:
	static std::wstring FromCodepage(std::string_view s, int32 codepage = 0) noexcept;
	static std::string AsCodepage(std::wstring_view s, int32 codepage = 0) noexcept;

public:
	static consteval size_t IOOverlappedDefaultSize() noexcept { return 64; }
	static void InitializeIOCompletionPort(size_t* iocp);
	static void BindIOCompletionPort(size_t* iocp, void* nativeHandle);
	static void DestroyIOCompletionPort(size_t& iocp) noexcept;
	static IOCompletionOverlapped* GetQueuedCompletionStatus(size_t& iocp, uint32 timeout, size_t* transferred) noexcept;
	static bool PostQueuedCompletionStatus(size_t& iocp, IOCompletionOverlapped* overlapped, size_t resolved) noexcept;

public:
	static void CreateAsyncFile(size_t* handle, String filename, EFileAccessMode accessMode, EFileSharedMode sharedMode);
	static void CloseAsyncFile(size_t handle) noexcept;
	static void WriteAsyncFile(size_t handle, std::span<const uint8> buf, IOCompletionOverlapped* overlapped);
	static void ReadAsyncFile(size_t handle, std::span<uint8> buf, IOCompletionOverlapped* overlapped);
	static size_t GetAsyncFileSize(size_t handle);
};