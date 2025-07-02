// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/Linux/LinuxPlatformIO.h"

#if PLATFORM_LINUX

#define __ALLOW_PLATFORM_COMMON_H__

#include "AssertionMacros.h"
#include "SystemException.h"
#include "Threading/ThreadPool.h"
#include "Threading/CancellationToken.h"
#include "Threading/CancellationTokenSource.h"
#include "Platform/PlatformCommon.h"
#include "LinuxStandardStreamTextWriter.h"
#include "IO/IOCompletionOverlapped.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <cerrno>
#include <limits>
#include <span>
#include <liburing.h>

namespace Ayla
{
    class IOCompletionPort
    {
    private:
        static constexpr uint32 kUringQueueDepth = 64;
        io_uring m_Ring;

        std::queue<std::pair<IOCompletionOverlapped*, size_t>> m_Overlaps;
        std::mutex m_Mutex;
        std::condition_variable m_Cond;

        std::mutex m_CancellationLock;
        CancellationTokenSource m_DispatchCancel;

    public:
        IOCompletionPort()
        {
            io_uring_queue_init((unsigned int)kUringQueueDepth, &m_Ring, 0);
            std::thread([this]() { this->io_uring_dispatch(this->m_DispatchCancel.GetToken()); }).detach();
        }

        ~IOCompletionPort() noexcept
        {
            auto lock = std::unique_lock{ m_CancellationLock };
            m_DispatchCancel.Cancel();
            lock.unlock();
            io_uring_queue_exit(&m_Ring);
        }

        bool DispatchQueuedCompletionStatus(const TimeSpan& dur) noexcept
        {
            auto lock = std::unique_lock{ m_Mutex };
            if (m_Overlaps.empty())
            {
                m_Cond.wait_for(lock, (std::chrono::nanoseconds)dur);
            }

            if (m_Overlaps.empty())
            {
                return false;
            }

            auto [overlap, res] = m_Overlaps.front();
            m_Overlaps.pop();
            lock.unlock();

            if (res >= 0)
            {
                overlap->Complete(res);
            }
            else
            {
                overlap->Failed(res);
            }

            return true;
        }
        
        void io_uring_dispatch(CancellationToken cancellationToken)
        {
            while (true)
            {
                auto lock = std::unique_lock{ m_CancellationLock };
                cancellationToken.ThrowIfCancellationRequested();
                io_uring_cqe* cqe = nullptr;
                int i_errno = io_uring_wait_cqe(&this->m_Ring, &cqe);
                lock.unlock();

                if (i_errno == 0)
                {
                    auto* overlap = (IOCompletionOverlapped*)cqe->user_data;
                    auto lock = std::unique_lock{ m_Mutex };
                    m_Overlaps.emplace(overlap, (size_t)cqe->res);
                    io_uring_cqe_seen(&this->m_Ring, cqe);
                    m_Cond.notify_one();
                }
                else
                {
                    PlatformProcess::OutputDebugString(String::Format(TEXT("i_errno = {}"), i_errno));
                }
            }
        }
    };

    TextWriter& LinuxPlatformIO::GetStandardOutput() noexcept
    {
        static LinuxStandardStreamTextWriter writer(STDOUT_FILENO);
        return writer;
    }

    TextWriter& LinuxPlatformIO::GetStandardError() noexcept
    {
        static LinuxStandardStreamTextWriter writer(STDERR_FILENO);
        return writer;
    }

    void LinuxPlatformIO::InitializeIOCPHandle(void*& Handle) noexcept
    {
        const uint32 kUringQueueDepth = 64;
        auto* completionPort = new IOCompletionPort();
        Handle = completionPort;
    }

    void LinuxPlatformIO::DestroyIOCPHandle(void* Handle) noexcept
    {
        delete reinterpret_cast<IOCompletionPort*>(Handle);
    }

    void LinuxPlatformIO::BindIOHandle(void* Handle, void* Socket) noexcept
    {
        PLATFORM_UNREFERENCED_PARAMETER(Handle);
        PLATFORM_UNREFERENCED_PARAMETER(Socket);
    }

    void LinuxPlatformIO::UnbindIOHandle(void* Handle, void* Socket) noexcept
    {
        PLATFORM_UNREFERENCED_PARAMETER(Handle);
        PLATFORM_UNREFERENCED_PARAMETER(Socket);
    }

    bool LinuxPlatformIO::DispatchQueuedCompletionStatus(void* Handle, const TimeSpan& Dur) noexcept
    {
        auto iocp = reinterpret_cast<IOCompletionPort*>(Handle);
        return iocp->DispatchQueuedCompletionStatus(Dur);
    }

    bool LinuxPlatformIO::DispatchQueuedCompletionStatus(void* Handle) noexcept
    {
        return DispatchQueuedCompletionStatus(Handle, TimeSpan::FromMilliseconds(0));
    }

    void LinuxPlatformIO::OpenFileHandle(void*& Handle, String InFilename, FileMode InFileMode, FileAccessMode InAccessMode, FileSharedMode InSharedMode) noexcept
    {
        int flags = 0;
        switch (InFileMode)
        {
        case FileMode::CreateNew: flags |= O_CREAT | O_EXCL; break;
        case FileMode::Create:    flags |= O_CREAT | O_TRUNC; break;
        case FileMode::Open:      break;
        case FileMode::OpenOrCreate: flags |= O_CREAT; break;
        case FileMode::Truncate:  flags |= O_TRUNC; break;
        }

        switch (InAccessMode)
        {
        case FileAccessMode::Read:    flags |= O_RDONLY; break;
        case FileAccessMode::Write:   flags |= O_WRONLY; break;
        case FileAccessMode::Append:  flags |= O_APPEND | O_CREAT; break;
        case (FileAccessMode::Read | FileAccessMode::Write):
            flags |= O_RDWR;
            break;
        }

        int fd = open(InFilename.AsCodepage().c_str(), flags, 0666);
        if (fd == -1)
        {
            Handle = nullptr;
            return;
        }

        Handle = reinterpret_cast<void*>(static_cast<intptr_t>(fd));
    }

    bool LinuxPlatformIO::CloseFileHandle(void* Handle) noexcept
    {
        int fd = static_cast<int>(reinterpret_cast<intptr_t>(Handle));
        return close(fd) == 0;
    }

    bool LinuxPlatformIO::FlushFileBuffers(void* Handle) noexcept
    {
        int fd = static_cast<int>(reinterpret_cast<intptr_t>(Handle));
        return fsync(fd) == 0;
    }

    bool LinuxPlatformIO::SetFileSeekPointer(void* Handle, int64 Seekpos, SeekOrigin InOrigin) noexcept
    {
        int fd = static_cast<int>(reinterpret_cast<intptr_t>(Handle));
        int whence = SEEK_SET;
        switch (InOrigin)
        {
        case SeekOrigin::Begin:   whence = SEEK_SET; break;
        case SeekOrigin::Current: whence = SEEK_CUR; break;
        case SeekOrigin::End:     whence = SEEK_END; break;
        }
        return lseek(fd, Seekpos, whence) != -1;
    }

    int64 LinuxPlatformIO::GetFileSize(void* Handle) noexcept
    {
        int fd = static_cast<int>(reinterpret_cast<intptr_t>(Handle));
        struct stat st;
        if (fstat(fd, &st) == -1)
        {
            return 0;
        }
        return static_cast<int64>(st.st_size);
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
        int fd = static_cast<int>(reinterpret_cast<intptr_t>(Handle));
        size_t totalWritten = 0;
        while (totalWritten < InBytes.size_bytes())
        {
            ssize_t written = write(fd, InBytes.data() + totalWritten, InBytes.size_bytes() - totalWritten);
            if (written < 0)
            {
                if (errno == EINTR)
                    continue;
                Overlap->Failed(errno);
                delete Overlap;
                return false;
            }
            totalWritten += static_cast<size_t>(written);
        }
        Overlap->Complete(totalWritten);
        delete Overlap;
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
        int fd = static_cast<int>(reinterpret_cast<intptr_t>(Handle));
        size_t totalRead = 0;
        while (totalRead < OutBytes.size_bytes())
        {
            ssize_t readBytes = read(fd, OutBytes.data() + totalRead, OutBytes.size_bytes() - totalRead);
            if (readBytes < 0)
            {
                if (errno == EINTR)
                    continue;
                Overlap->Failed(errno);
                delete Overlap;
                return false;
            }
            if (readBytes == 0)
            {
                break; // EOF
            }
            totalRead += static_cast<size_t>(readBytes);
        }
        Overlap->Complete(totalRead);
        delete Overlap;
        return true;
    }
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif
