// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/FileStream.h"
#include "IO/IOFileOverlapped.h"
#include "Exceptions/SystemException.h"
#include "Threading/ThreadPool.h"

#if PLATFORM_WINDOWS
#include "Misc/WindowsPlatformMisc.h"
#endif

FileStream::FileStream(String filename, EFileAccessMode accessMode, EFileSharedMode sharedMode, EFileMode fileMode)
{
#if PLATFORM_WINDOWS
	_handle = CreateFileW(filename.c_str(), (DWORD)accessMode, (DWORD)sharedMode, NULL, (DWORD)fileMode, FILE_FLAG_OVERLAPPED, NULL);
	if (_handle == nullptr)
	{
		throw SystemException(GetLastError());
	}

	ThreadPool::BindHandle(_handle);
#endif
}

FileStream::~FileStream() noexcept
{
	if (_handle)
	{
		Close();
	}
}

void FileStream::Flush()
{
#if PLATFORM_WINDOWS
	if (FlushFileBuffers(_handle) == FALSE)
	{
		throw SystemException(GetLastError());
	}
#endif
}

void FileStream::Close()
{
#if PLATFORM_WINDOWS
	if (_handle)
	{
		if (CloseHandle(_handle) == FALSE)
		{
			throw SystemException(GetLastError());
		}
		_handle = nullptr;
	}
#endif
}

void FileStream::Write(std::span<const uint8> bytes)
{
	WriteAsync(bytes).GetResult();
}

Task<> FileStream::WriteAsync(std::span<const uint8> bytes, std::stop_token cancellationToken)
{
#if PLATFORM_WINDOWS
	auto tcs = TaskCompletionSource<>::Create<size_t>(cancellationToken);
	auto* ptr = new IOFileOverlapped(tcs);
	if (::WriteFile(_handle, bytes.data(), (DWORD)bytes.size_bytes(), NULL, (LPOVERLAPPED)ptr->ToOverlapped()) == FALSE)
	{
		if (DWORD err = GetLastError(); err != ERROR_IO_PENDING)
		{
			auto e = std::make_exception_ptr(SystemException(err));
			if (ptr->Failed(e))
			{
				delete ptr;
			}
			std::rethrow_exception(e);
		}
	}
	return tcs.GetTask();
#endif
}

size_t FileStream::Read(std::span<uint8> bytes)
{
	return ReadAsync(bytes).GetResult();
}

Task<size_t> FileStream::ReadAsync(std::span<uint8> bytes, std::stop_token cancellationToken)
{
#if PLATFORM_WINDOWS
	auto tcs = TaskCompletionSource<>::Create<size_t>(cancellationToken);
	auto* ptr = new IOFileOverlapped(tcs);
	if (::ReadFile(_handle, bytes.data(), (DWORD)bytes.size_bytes(), NULL, (LPOVERLAPPED)ptr->ToOverlapped()) == FALSE)
	{
		if (DWORD err = GetLastError(); err != ERROR_IO_PENDING)
		{
			auto e = std::make_exception_ptr(SystemException(err));
			if (ptr->Failed(e))
			{
				delete ptr;
			}
			std::rethrow_exception(e);
		}
	}
	return tcs.GetTask();
#endif
}