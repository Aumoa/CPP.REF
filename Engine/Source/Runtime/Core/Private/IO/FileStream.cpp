// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IO/FileStream.h"
#include "IO/IOCompletionOverlapped.h"
#include "System/SystemException.h"
#include "Platform/PlatformIO.h"
#include "Platform/PlatformMisc.h"
#include "Threading/Tasks/TaskCompletionSource.h"

FileStream::FileStream(String InFilename, EFileMode InFileMode, EFileAccessMode InAccessMode, EFileSharedMode InSharedMode)
{
	PlatformIO::OpenFileHandle(FileHandle, InFilename, InFileMode, InAccessMode, InSharedMode);
	if (FileHandle == nullptr)
	{
		throw SystemException(PlatformMisc::GetLastError());
	}
}

FileStream::~FileStream() noexcept
{
	if (ensureMsgf(FileHandle == nullptr, TEXT("File handle is not closed. Call 'Close()' function to close handle before object is finalized.")) == false)
	{
		Close();
	}
}

void FileStream::Flush()
{
	if (PlatformIO::FlushFileBuffers(FileHandle) == false)
	{
		throw SystemException(PlatformMisc::GetLastError());
	}
}

void FileStream::Close()
{
	if (FileHandle)
	{
		if (PlatformIO::CloseFileHandle(FileHandle) == false)
		{
			throw SystemException(PlatformMisc::GetLastError());
		}
		FileHandle = nullptr;
	}
}

void FileStream::Write(std::span<const uint8> InBytes)
{
	WriteAsync(InBytes).GetResult();
}

Task<> FileStream::WriteAsync(std::span<const uint8> InBytes, CancellationToken InCancellationToken)
{
	auto TCS = TaskCompletionSource<>::Create<size_t>(InCancellationToken);
	auto Ptr = new IOCompletionOverlapped(PlatformIO::FileIOWrittenAction(TCS, IOCPWriteBuf));
	memcpy(Ptr->ToOverlapped(), IOCPWriteBuf, sizeof(IOCPWriteBuf));

	if (PlatformIO::WriteFile(FileHandle, InBytes, std::move(Ptr)) == false)
	{
		Ptr->Failed(PlatformMisc::GetLastError());
		delete Ptr;
	}

	return TCS.GetTask();
}

void FileStream::Seek(int64 InSeekpos, ESeekOrigin InOrigin)
{
	if (PlatformIO::SetFileSeekPointer(FileHandle, InSeekpos, InOrigin) == false)
	{
		throw SystemException(PlatformMisc::GetLastError());
	}
}

size_t FileStream::Read(std::span<uint8> OutBytes)
{
	return ReadAsync(OutBytes).GetResult();
}

Task<size_t> FileStream::ReadAsync(std::span<uint8> OutBytes, CancellationToken InCancellationToken)
{
	auto TCS = TaskCompletionSource<>::Create<size_t>(InCancellationToken);
	auto Ptr = new IOCompletionOverlapped(PlatformIO::FileIOReadAction(TCS, IOCPReadBuf));
	memcpy(Ptr->ToOverlapped(), IOCPReadBuf, sizeof(IOCPReadBuf));

	if (PlatformIO::ReadFile(FileHandle, OutBytes, Ptr) == false)
	{
		Ptr->Failed(PlatformMisc::GetLastError());
		delete Ptr;
	}

	return TCS.GetTask();
}

size_t FileStream::GetLength() const
{
	return (size_t)PlatformIO::GetFileSize(FileHandle);
}