// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IO/FileStream.h"
#include "IO/IOCompletionOverlapped.h"
#include "SystemException.h"
#include "Platform/PlatformIO.h"
#include "Platform/PlatformMisc.h"
#include "Threading/Tasks/TaskCompletionSource.h"

namespace Ayla
{
	FileStream::FileStream(String InFilename, FileMode InFileMode, FileAccessMode InAccessMode, FileSharedMode InSharedMode)
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

	Task<> FileStream::WriteAsync(std::span<const uint8> InBytes, std::stop_token InCancellationToken)
	{
		auto tcs = TaskCompletionSource<>::Create<size_t>(InCancellationToken);
		auto ptr = new IOCompletionOverlapped();
		
		auto completionCallback = PlatformIO::FileIOWrittenAction(tcs, IOCPWriteBuf);
		ptr->SetOnCompletion([ptr, cc = std::move(completionCallback)](size_t size, int32 r) mutable
		{
			cc(ptr, size, r);
			delete ptr;
		});

		memcpy(ptr->ToOverlapped(), IOCPWriteBuf, sizeof(IOCPWriteBuf));

		if (PlatformIO::WriteFile(FileHandle, InBytes, ptr) == false)
		{
			ptr->Failed(PlatformMisc::GetLastError());
			delete ptr;
		}

		return tcs.GetTask();
	}

	void FileStream::Seek(int64 InSeekpos, SeekOrigin InOrigin)
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

	Task<size_t> FileStream::ReadAsync(std::span<uint8> OutBytes, std::stop_token InCancellationToken)
	{
		auto tcs = TaskCompletionSource<>::Create<size_t>(InCancellationToken);
		auto ptr = new IOCompletionOverlapped();

		auto completionCallback = PlatformIO::FileIOReadAction(tcs, IOCPReadBuf);
		ptr->SetOnCompletion([ptr, cc = std::move(completionCallback)](size_t size, int32 r) mutable
		{
			cc(ptr, size, r);
			delete ptr;
		});

		memcpy(ptr->ToOverlapped(), IOCPReadBuf, sizeof(IOCPReadBuf));

		if (PlatformIO::ReadFile(FileHandle, OutBytes, ptr) == false)
		{
			ptr->Failed(PlatformMisc::GetLastError());
			delete ptr;
		}

		return tcs.GetTask();
	}

	size_t FileStream::GetLength() const
	{
		return (size_t)PlatformIO::GetFileSize(FileHandle);
	}
}