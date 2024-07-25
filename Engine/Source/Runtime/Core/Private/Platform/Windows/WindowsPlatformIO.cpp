// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:WindowsPlatformIO;

export import :Forward;
export import :GenericPlatformIO;
export import :Action;
export import :TimeSpan;
export import :TextWriter;
export import :FileMode;
export import :FileAccessMode;
export import :FileSharedMode;
export import :SeekOrigin;
export import :TaskCompletionSource;

#if PLATFORM_WINDOWS

export class CORE_API WindowsPlatformIO : public GenericPlatformIO
{
public:
	static constexpr size_t OVERLAPPED_SIZE = 64;

	static TextWriter& GetStandardOutput() noexcept;
	static TextWriter& GetStandardError() noexcept;

	static void InitializeIOCPHandle(void*& Handle) noexcept;
	static void DestroyIOCPHandle(void* Handle) noexcept;
	static void BindIOHandle(void* Handle, void* Socket) noexcept;
	static void UnbindIOHandle(void* Handle, void* Socket) noexcept;

	static bool DispatchQueuedCompletionStatus(void* Handle, const TimeSpan& Dur) noexcept;
	static bool DispatchQueuedCompletionStatus(void* Handle) noexcept;

	static void OpenFileHandle(void*& Handle, String InFilename, EFileMode InFileMode, EFileAccessMode InAccessMode, EFileSharedMode InSharedMode) noexcept;
	static bool CloseFileHandle(void* Handle) noexcept;
	static bool FlushFileBuffers(void* Handle) noexcept;
	static bool SetFileSeekPointer(void* Handle, int64 Seekpos, ESeekOrigin InOrigin) noexcept;
	static int64 GetFileSize(void* Handle) noexcept;

	static Action<IOCompletionOverlapped*, size_t, int32> FileIOWrittenAction(TaskCompletionSource<size_t> TCS, void* WriteIO) noexcept;
	static bool WriteFile(void* Handle, std::span<const uint8> InBytes, IOCompletionOverlapped* Overlap) noexcept;

	static Action<IOCompletionOverlapped*, size_t, int32> FileIOReadAction(TaskCompletionSource<size_t> TCS, void* ReadIO) noexcept;
	static bool ReadFile(void* Handle, std::span<uint8> OutBytes, IOCompletionOverlapped* Overlap) noexcept;
};

#endif