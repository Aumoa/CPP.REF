// Copyright 2020-2023 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Platform/Windows/WindowsCore.h"

import Core.IO;

FileStream::FileStream(String InFilename, EFileAccessMode InAccessMode, EFileSharedMode InSharedMode, EFileMode InFileMode)
{
	FileHandle = CreateFileW(InFilename.c_str(), (DWORD)InAccessMode, (DWORD)InSharedMode, NULL, (DWORD)InFileMode, FILE_FLAG_OVERLAPPED, NULL);
	if (FileHandle == nullptr)
	{
		throw SystemException(GetLastError());
	}

	ThreadPool::BindHandle(FileHandle);
	WriteOverlapPointer = new OVERLAPPED{};
	ReadOverlapPointer = new OVERLAPPED{};
}

FileStream::~FileStream() noexcept
{
	if (WriteOverlapPointer)
	{
		delete (OVERLAPPED*)WriteOverlapPointer;
		WriteOverlapPointer = nullptr;
	}

	if (ReadOverlapPointer)
	{
		delete (OVERLAPPED*)ReadOverlapPointer;
		ReadOverlapPointer = nullptr;
	}

	if (ensureMsgf(FileHandle == nullptr, TEXT("File handle is not closed. Call 'Close()' function to close handle before object is finalized.")) == false)
	{
		Close();
	}
}

void FileStream::Flush()
{
	if (FlushFileBuffers(FileHandle) == FALSE)
	{
		throw SystemException(GetLastError());
	}
}

void FileStream::Close()
{
	if (FileHandle)
	{
		if (CloseHandle(FileHandle) == FALSE)
		{
			throw SystemException(GetLastError());
		}
		FileHandle = nullptr;
	}
}

void FileStream::Write(std::span<const uint8> InBytes)
{
	WriteAsync(InBytes).GetResult();
}

Task<> FileStream::WriteAsync(std::span<const uint8> InBytes, std::stop_token InCancellationToken)
{
	auto TCS = TaskCompletionSource<>::Create<size_t>(InCancellationToken);
	auto* Ptr = new IOCompletionOverlapped([TCS, this](IOCompletionOverlapped* Self, size_t Written, int32 ErrorCode) mutable
	{
		if (ErrorCode)
		{
			TCS.SetException(SystemException(ErrorCode));
		}
		else
		{
			TCS.SetResult(Written);
		}

		memcpy(WriteOverlapPointer, Self->ToOverlapped(), sizeof(OVERLAPPED));
	});

	memcpy(Ptr->ToOverlapped(), WriteOverlapPointer, sizeof(OVERLAPPED));

	if (::WriteFile(FileHandle, InBytes.data(), (DWORD)InBytes.size_bytes(), NULL, (LPOVERLAPPED)Ptr->ToOverlapped()) == FALSE)
	{
		if (DWORD LastError = GetLastError(); LastError != ERROR_IO_PENDING)
		{
			Ptr->Failed(LastError);
			delete Ptr;
		}
	}

	return TCS.GetTask();
}

void FileStream::Seek(int64 InSeekpos, ESeekOrigin InOrigin)
{
	if (::SetFilePointer(FileHandle, (DWORD)InSeekpos, ((PLONG)&InSeekpos) + 1, (int32)InOrigin) == INVALID_SET_FILE_POINTER)
	{
		throw SystemException(::GetLastError());
	}
}

size_t FileStream::Read(std::span<uint8> OutBytes)
{
	return ReadAsync(OutBytes).GetResult();
}

Task<size_t> FileStream::ReadAsync(std::span<uint8> OutBytes, std::stop_token InCancellationToken)
{
	auto TCS = TaskCompletionSource<>::Create<size_t>(InCancellationToken);
	auto* Ptr = new IOCompletionOverlapped([TCS, this](IOCompletionOverlapped* Self, size_t Read, int32 ErrorCode) mutable
	{
		if (ErrorCode)
		{
			TCS.SetException(SystemException(ErrorCode));
		}
		else
		{
			TCS.SetResult(Read);
		}

		memcpy(ReadOverlapPointer, Self->ToOverlapped(), sizeof(OVERLAPPED));
	});

	memcpy(Ptr->ToOverlapped(), ReadOverlapPointer, sizeof(OVERLAPPED));

	if (::ReadFile(FileHandle, OutBytes.data(), (DWORD)OutBytes.size_bytes(), NULL, (LPOVERLAPPED)Ptr->ToOverlapped()) == FALSE)
	{
		if (DWORD LastError = GetLastError(); LastError != ERROR_IO_PENDING)
		{
			Ptr->Failed(LastError);
			delete Ptr;
		}
	}

	return TCS.GetTask();
}

size_t FileStream::GetLength() const
{
	DWORD HighCode = 0;
	DWORD LowCode = GetFileSize(FileHandle, &HighCode);
	return (size_t)LowCode | (size_t)HighCode << 32;
}

#endif