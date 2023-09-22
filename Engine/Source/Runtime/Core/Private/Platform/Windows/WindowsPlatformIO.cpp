// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Platform/Windows/WindowsPlatformIO.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "System/AssertionMacros.h"
#include "System/SystemException.h"
#include "Threading/ThreadPool.h"
#include "Platform/PlatformCommon.h"
#include "WindowsStandardStreamTextWriter.h"
#include "IO/IOCompletionOverlapped.h"

inline constexpr HANDLE GetHANDLE(void* Handle)
{
	if constexpr (sizeof(HANDLE) <= sizeof(void*))
	{
		return (HANDLE)Handle;
	}
	else
	{
		return *(HANDLE*)Handle;
	}
}

inline constexpr SOCKET GetSOCKET(void* Socket)
{
	if constexpr (sizeof(SOCKET) <= sizeof(void*))
	{
		return (SOCKET)Socket;
	}
	else
	{
		return *(SOCKET*)Socket;
	}
}

namespace
{
	void static_assert_trigger()
	{
		static_assert(WindowsPlatformIO::OVERLAPPED_SIZE >= sizeof(OVERLAPPED), "OVERLAPPED size is not compatible.");
	}
}

TextWriter& WindowsPlatformIO::GetStandardOutput() noexcept
{
	static WindowsStandardStreamTextWriter Writer(STD_OUTPUT_HANDLE);
	return Writer;
}

TextWriter& WindowsPlatformIO::GetStandardError() noexcept
{
	static WindowsStandardStreamTextWriter Writer(STD_ERROR_HANDLE);
	return Writer;
}

void WindowsPlatformIO::InitializeIOCPHandle(void*& Handle) noexcept
{
	if constexpr (sizeof(HANDLE) <= sizeof(void*))
	{
		(HANDLE&)Handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, MAXDWORD);
	}
	else
	{
		Handle = new HANDLE
		{
			CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, MAXDWORD)
		};
	}
	check(GetHANDLE(Handle));
}

void WindowsPlatformIO::DestroyIOCPHandle(void* Handle) noexcept
{
	if constexpr (sizeof(HANDLE) < sizeof(void*))
	{
		__nop();
	}
	else
	{
		delete (HANDLE*)Handle;
	}
}

void WindowsPlatformIO::BindIOHandle(void* Handle, void* Socket) noexcept
{
	HANDLE hOk = CreateIoCompletionPort((HANDLE)GetSOCKET(Socket), GetHANDLE(Handle), (ULONG_PTR)Socket, 0);
	checkf(hOk, SystemException(GetLastError()).GetMessage());
}

void WindowsPlatformIO::UnbindIOHandle(void* Handle, void* Socket) noexcept
{
	check(Handle);
	check(Socket);
	// Windows platform need not unbind IOCP handle.
}

bool WindowsPlatformIO::DispatchQueuedCompletionStatus(void* Handle, const TimeSpan& Dur) noexcept
{
	DWORD NumberOfBytes;
	ULONG_PTR CompletionKey = 0;
	LPOVERLAPPED Overlapped;
	BOOL bStatus = GetQueuedCompletionStatus(GetHANDLE(Handle), &NumberOfBytes, &CompletionKey, &Overlapped, Dur.GetTotalMilliseconds<DWORD>());

	if (CompletionKey != 0)
	{
		auto* Ptr = IOCompletionOverlapped::FromOverlapped(Overlapped);
		ThreadPool::QueueUserWorkItem([Ptr, bStatus, NumberOfBytes, ErrorCode = ::WSAGetLastError()]()
		{
			if (bStatus)
			{
				Ptr->Complete((size_t)NumberOfBytes);
			}
			else
			{
				Ptr->Failed(ErrorCode);
			}
		});
		return true;
	}
	else
	{
		return bStatus == TRUE;
	}
}

bool WindowsPlatformIO::DispatchQueuedCompletionStatus(void* Handle) noexcept
{
	return DispatchQueuedCompletionStatus(Handle, TimeSpan::FromMilliseconds(INFINITE));
}

void WindowsPlatformIO::OpenFileHandle(void*& Handle, String InFilename, EFileMode InFileMode, EFileAccessMode InAccessMode, EFileSharedMode InSharedMode) noexcept
{
	HANDLE hFile = CreateFileW(InFilename.c_str(), (DWORD)InAccessMode, (DWORD)InSharedMode, NULL, (DWORD)InFileMode, FILE_FLAG_OVERLAPPED, NULL);
	if (hFile == NULL)
	{
		Handle = nullptr;
		return;
	}

	Handle = hFile;
	ThreadPool::BindHandle(Handle);
}

bool WindowsPlatformIO::CloseFileHandle(void* Handle) noexcept
{
	ThreadPool::UnbindHandle(Handle);
	return CloseHandle((HANDLE)Handle) == TRUE;
}

bool WindowsPlatformIO::FlushFileBuffers(void* Handle) noexcept
{
	return ::FlushFileBuffers((HANDLE)Handle) == TRUE;
}

bool WindowsPlatformIO::SetFileSeekPointer(void* Handle, int64 Seekpos, ESeekOrigin InOrigin) noexcept
{
	return ::SetFilePointerEx((HANDLE)Handle, reinterpret_cast<LARGE_INTEGER&>(Seekpos), NULL, (DWORD)InOrigin);
}

int64 WindowsPlatformIO::GetFileSize(void* Handle) noexcept
{
	LARGE_INTEGER LargeInteger;
	if (::GetFileSizeEx((HANDLE)Handle, &LargeInteger) == FALSE)
	{
		return 0;
	}

	return (int64)LargeInteger.QuadPart;
}

Action<IOCompletionOverlapped*, size_t, int32> WindowsPlatformIO::FileIOWrittenAction(TaskCompletionSource<size_t> TCS, void* WriteIO) noexcept
{
	return [TCS, WriteIO](IOCompletionOverlapped* Self, size_t Written, int32 ErrorCode)
	{
		auto ScopedPtr = std::unique_ptr<IOCompletionOverlapped>(Self);

		auto* Overlap = reinterpret_cast<OVERLAPPED*>(Self->ToOverlapped());
		(ULONG_PTR&)Overlap->Pointer += Written;
		memcpy(WriteIO, Overlap, sizeof(OVERLAPPED));

		if (ErrorCode)
		{
			TCS.SetException(std::make_exception_ptr(SystemException(ErrorCode)));
		}
		else
		{
			TCS.SetResult(Written);
		}
	};
}

bool WindowsPlatformIO::WriteFile(void* Handle, std::span<const uint8> InBytes, IOCompletionOverlapped* Overlap) noexcept
{
	auto Overlapped = reinterpret_cast<OVERLAPPED*>(Overlap->ToOverlapped());
	size_t Seekpos = 0;

	while (Seekpos < InBytes.size_bytes())
	{
		DWORD Write = static_cast<DWORD>(Math::Min(InBytes.size_bytes(), (size_t)std::numeric_limits<DWORD>::max()));
		DWORD Written;
		if (::WriteFile((HANDLE)Handle, reinterpret_cast<LPCVOID>(InBytes.data() + Seekpos), Write, &Written, Overlapped) == FALSE)
		{
			if (::GetLastError() == ERROR_IO_PENDING)
			{
				return true;
			}

			return false;
		}

		Seekpos += (size_t)Write;
	}

	Overlap->Complete(Seekpos);
	delete Overlap;

	return true;
}

Action<IOCompletionOverlapped*, size_t, int32> WindowsPlatformIO::FileIOReadAction(TaskCompletionSource<size_t> TCS, void* ReadIO) noexcept
{
	return [TCS, ReadIO](IOCompletionOverlapped* Self, size_t Read, int32 ErrorCode)
	{
		auto ScopedPtr = std::unique_ptr<IOCompletionOverlapped>(Self);

		auto* Overlap = reinterpret_cast<OVERLAPPED*>(Self->ToOverlapped());
		(ULONG_PTR&)Overlap->Pointer += Read;
		memcpy(ReadIO, Overlap, sizeof(OVERLAPPED));

		if (ErrorCode && ErrorCode != ERROR_HANDLE_EOF)
		{
			TCS.SetException(std::make_exception_ptr(SystemException(ErrorCode)));
		}
		else
		{
			TCS.SetResult(Read);
		}
	};
}

bool WindowsPlatformIO::ReadFile(void* Handle, std::span<uint8> OutBytes, IOCompletionOverlapped* Overlap) noexcept
{
	auto Overlapped = reinterpret_cast<OVERLAPPED*>(Overlap->ToOverlapped());
	DWORD Reads;

	if (::ReadFile((HANDLE)Handle, OutBytes.data(), static_cast<DWORD>(Math::Min(OutBytes.size_bytes(), (size_t)std::numeric_limits<DWORD>::max())), &Reads, Overlapped) == FALSE)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			return true;
		}

		return false;
	}

	Overlap->Complete((size_t)Reads);
	delete Overlap;

	return true;
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif