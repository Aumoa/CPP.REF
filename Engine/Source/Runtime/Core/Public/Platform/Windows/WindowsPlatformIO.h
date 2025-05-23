// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include "Platform/Generic/GenericPlatformIO.h"
#include "Action.h"
#include "TimeSpan.h"
#include "IO/TextWriter.h"
#include "IO/FileMode.h"
#include "IO/FileAccessMode.h"
#include "IO/FileSharedMode.h"
#include "IO/SeekOrigin.h"
#include "Threading/Tasks/TaskCompletionSource.h"

namespace Ayla
{
	class IOCompletionOverlapped;

	struct CORE_API WindowsPlatformIO : public GenericPlatformIO
	{
		static constexpr size_t OVERLAPPED_SIZE = 64;

		static TextWriter& GetStandardOutput() noexcept;
		static TextWriter& GetStandardError() noexcept;

		static void InitializeIOCPHandle(void*& Handle) noexcept;
		static void DestroyIOCPHandle(void* Handle) noexcept;
		static void BindIOHandle(void* Handle, void* Socket) noexcept;
		static void UnbindIOHandle(void* Handle, void* Socket) noexcept;

		static bool DispatchQueuedCompletionStatus(void* Handle, const TimeSpan& Dur) noexcept;
		static bool DispatchQueuedCompletionStatus(void* Handle) noexcept;

		static void OpenFileHandle(void*& Handle, String InFilename, FileMode InFileMode, FileAccessMode InAccessMode, FileSharedMode InSharedMode) noexcept;
		static bool CloseFileHandle(void* Handle) noexcept;
		static bool FlushFileBuffers(void* Handle) noexcept;
		static bool SetFileSeekPointer(void* Handle, int64 Seekpos, SeekOrigin InOrigin) noexcept;
		static int64 GetFileSize(void* Handle) noexcept;

		static Action<IOCompletionOverlapped*, size_t, int32> FileIOWrittenAction(TaskCompletionSource<size_t> TCS, void* WriteIO) noexcept;
		static bool WriteFile(void* Handle, std::span<const uint8> InBytes, IOCompletionOverlapped* Overlap) noexcept;

		static Action<IOCompletionOverlapped*, size_t, int32> FileIOReadAction(TaskCompletionSource<size_t> TCS, void* ReadIO) noexcept;
		static bool ReadFile(void* Handle, std::span<uint8> OutBytes, IOCompletionOverlapped* Overlap) noexcept;
	};

	using PlatformIO = WindowsPlatformIO;
}

#endif