// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Platform/Linux/LinuxPlatformIO.h"

#if PLATFORM_LINUX

#include "Platform/Linux/LinuxStandardStreamTextWriter.h"
#include "System/SystemException.h"
#include "IO/IOCompletionOverlapped.h"
#include <sys/epoll.h>
#include <fcntl.h>

inline constexpr int GetFD(void* Handle) noexcept
{
	return static_cast<int>(reinterpret_cast<size_t>(Handle));
}

TextWriter& LinuxPlatformIO::GetStandardOutput() noexcept
{
	static LinuxStandardStreamTextWriter StdoutWriter = LinuxStandardStreamTextWriter(0);
	return StdoutWriter;
}

TextWriter& LinuxPlatformIO::GetStandardError() noexcept
{
	static LinuxStandardStreamTextWriter StderrWriter = LinuxStandardStreamTextWriter(1);
	return StderrWriter;
}

void LinuxPlatformIO::InitializeIOCPHandle(void*& Handle) noexcept
{
	reinterpret_cast<size_t&>(Handle)
		= static_cast<size_t>(epoll_create(1));
	check(GetFD(Handle));
}

void LinuxPlatformIO::DestroyIOCPHandle(void* Handle) noexcept
{
	check(GetFD(Handle));
	close(GetFD(Handle));
}

void LinuxPlatformIO::BindIOHandle(void* Handle, void* Socket) noexcept
{
	epoll_event Evts;
	Evts.events = EPOLLIN | EPOLLET;
	Evts.data.fd = GetFD(Socket);
	int Err = epoll_ctl(GetFD(Handle), EPOLL_CTL_ADD, GetFD(Socket), &Evts);
	check(Err >= 0);
}

void LinuxPlatformIO::UnbindIOHandle(void* Handle, void* Socket) noexcept
{
	epoll_ctl(GetFD(Handle), EPOLL_CTL_DEL, GetFD(Socket), NULL);
}

bool LinuxPlatformIO::DispatchQueuedCompletionStatus(void* Handle, const TimeSpan& Dur) noexcept
{
	check(false);
	return false;
}

bool LinuxPlatformIO::DispatchQueuedCompletionStatus(void* Handle) noexcept
{
	check(false);
	return false;
}

void LinuxPlatformIO::OpenFileHandle(void*& Handle, String InFilename, EFileMode InFileMode, EFileAccessMode InAccessMode, EFileSharedMode InSharedMode) noexcept
{
	std::string MbChars = InFilename.string();

	const bool bRead = (InAccessMode & EFileAccessMode::Read) != 0;
	const bool bWrite = (InAccessMode & EFileAccessMode::Write) != 0;
	const bool bAppend = (InAccessMode & EFileAccessMode::Append) != 0;

	int Flags = 0;
	switch (InFileMode)
	{
	case EFileMode::CreateNew:
		Flags |= O_CREAT | O_EXCL;
		break;
	case EFileMode::Create:
		Flags |= O_CREAT | O_TRUNC;
		break;
	case EFileMode::Open:
		Flags |= 0;
		break;
	case EFileMode::OpenOrCreate:
		Flags |= O_CREAT;
		break;
	case EFileMode::Truncate:
		Flags |= O_TRUNC;
		break;
	}
	if (bAppend)
	{
		Flags |= O_APPEND;
	}
	if (bRead && bWrite)
	{
		Flags |= O_RDWR;
	}
	else if (bRead)
	{
		Flags |= O_RDONLY;
	}
	else if (bWrite)
	{
		Flags |= O_WRONLY;
	}
	int Mode = 0666;

	int Fd = open(MbChars.c_str(), Flags, Mode);
	check(Fd);

	reinterpret_cast<size_t&>(Handle) = static_cast<size_t>(Fd);
}

bool LinuxPlatformIO::CloseFileHandle(void* Handle) noexcept
{
	close(GetFD(Handle));
	return true;
}

bool LinuxPlatformIO::FlushFileBuffers(void* Handle) noexcept
{
	check(false);
	return false;
}

bool LinuxPlatformIO::SetFileSeekPointer(void* Handle, int64 Seekpos, ESeekOrigin InOrigin) noexcept
{
	check(false);
	return false;
}

int64 LinuxPlatformIO::GetFileSize(void* Handle) noexcept
{
	check(false);
	return 0;
}

Action<IOCompletionOverlapped*, size_t, int32> LinuxPlatformIO::FileIOWrittenAction(TaskCompletionSource<size_t> TCS, void* WriteIO) noexcept
{
	return [TCS, WriteIO](IOCompletionOverlapped* Self, size_t Written, int32 ErrorCode)
	{
		auto ScopedPtr = std::unique_ptr<IOCompletionOverlapped>(Self);
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

bool LinuxPlatformIO::WriteFile(void* Handle, std::span<const uint8> InBytes, IOCompletionOverlapped* Overlap) noexcept
{
	ssize_t Written = write(GetFD(Handle), InBytes.data(), InBytes.size_bytes());
	Overlap->Complete(static_cast<size_t>(Written));
	return true;
}

Action<IOCompletionOverlapped*, size_t, int32> LinuxPlatformIO::FileIOReadAction(TaskCompletionSource<size_t> TCS, void* ReadIO) noexcept
{
	return [TCS, ReadIO](IOCompletionOverlapped* Self, size_t Read, int32 ErrorCode)
	{
		auto ScopedPtr = std::unique_ptr<IOCompletionOverlapped>(Self);
		if (ErrorCode)
		{
			TCS.SetException(std::make_exception_ptr(SystemException(ErrorCode)));
		}
		else
		{
			TCS.SetResult(Read);
		}
	};
}

bool LinuxPlatformIO::ReadFile(void* Handle, std::span<uint8> OutBytes, IOCompletionOverlapped* Overlap) noexcept
{
	ssize_t Reads = read(GetFD(Handle), OutBytes.data(), OutBytes.size_bytes());
	Overlap->Complete(static_cast<size_t>(Reads));
	return true;
}

#endif