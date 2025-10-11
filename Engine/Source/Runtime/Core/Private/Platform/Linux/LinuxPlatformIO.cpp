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
        CancellationTokenSource m_DispatchCancel = CancellationTokenSource::Create();

    public:
        struct scoped_sqe
        {
            IOCompletionPort* m_CompletionPort = nullptr;
            io_uring_sqe* m_sqe = nullptr;

            scoped_sqe(IOCompletionPort* completionPort)
                : m_CompletionPort{ completionPort }
            {
                completionPort->m_Mutex.lock();
                m_sqe = io_uring_get_sqe(&completionPort->m_Ring);
            }

            scoped_sqe(scoped_sqe&& rhs) noexcept
                : m_CompletionPort{ rhs.m_CompletionPort }
                , m_sqe{ rhs.m_sqe }
            {
                rhs.m_CompletionPort = nullptr;
                rhs.m_sqe = nullptr;
            }

            ~scoped_sqe() noexcept
            {
                if (m_CompletionPort != nullptr)
                {
                    io_uring_submit(&m_CompletionPort->m_Ring);
                    m_CompletionPort->m_Mutex.unlock();
                }
            }

            inline operator io_uring_sqe*() const noexcept { return m_sqe; }
        };

    public:
        IOCompletionPort()
        {
            io_uring_queue_init((unsigned int)kUringQueueDepth, &m_Ring, 0);
            std::thread([this]() { this->io_uring_dispatch(this->m_DispatchCancel.GetToken()); }).detach();
        }

        ~IOCompletionPort() noexcept
        {
            io_uring_queue_exit(&m_Ring);
        }

        bool DispatchQueuedCompletionStatus(const TimeSpan& dur) noexcept
        {
            auto lock = std::unique_lock{ m_Mutex };
            auto cancellationToken = m_DispatchCancel.GetToken();
            if (m_Overlaps.empty())
            {
                auto pred = [this, &cancellationToken]()
                {
                    return m_Overlaps.empty() == false || cancellationToken.IsCancellationRequested();
                };
                auto len = (std::chrono::nanoseconds)dur;
                if (len == 0ns)
                {
                    m_Cond.wait(lock, pred);
                }
                else
                {
                    m_Cond.wait_for(lock, len, pred);
                }
            }

            if (m_DispatchCancel.GetToken().IsCancellationRequested())
            {
                return false;
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

        void QueueInterruptSignal() noexcept
        {
            auto lock = std::unique_lock{ m_Mutex };
            m_DispatchCancel.Cancel();
            m_Cond.notify_all();
        }
        
        void io_uring_dispatch(CancellationToken cancellationToken)
        {
            while (true)
            {
                auto lock = std::unique_lock{ m_CancellationLock };
                if (cancellationToken.IsCancellationRequested())
                {
                    break;
                }

                io_uring_cqe* cqe = nullptr;
                int i_errno = io_uring_wait_cqe(&this->m_Ring, &cqe);
                lock.unlock();

                if (i_errno == -EINTR)
                {
                    continue;
                }
                else if (i_errno == 0)
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

        scoped_sqe get_scoped_sqe() noexcept
        {
            return scoped_sqe(this);
        }
    };

    struct SocketHandle
    {
        int m_fd;
        IOCompletionPort* m_CompletionPort;
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
        auto* socketHandle = reinterpret_cast<SocketHandle*>(Socket);
        socketHandle->m_CompletionPort = reinterpret_cast<IOCompletionPort*>(Handle);
    }

    void LinuxPlatformIO::UnbindIOHandle(void* Handle, void* Socket) noexcept
    {
        auto* socketHandle = reinterpret_cast<SocketHandle*>(Socket);
        check(socketHandle->m_CompletionPort == Handle);
        socketHandle->m_CompletionPort = nullptr;
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

    void LinuxPlatformIO::QueueInterruptSignal(void* handle, int32 size) noexcept
    {
        auto iocp = reinterpret_cast<IOCompletionPort*>(handle);
        iocp->QueueInterruptSignal();
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

        Handle = new SocketHandle
        {
            .m_fd = fd,
            .m_CompletionPort = nullptr
        };
        ThreadPool::BindHandle(Handle);
    }

    bool LinuxPlatformIO::CloseFileHandle(void* Handle) noexcept
    {
        auto socketHandle = reinterpret_cast<SocketHandle*>(Handle);
        ThreadPool::UnbindHandle(socketHandle);
        bool closed = close(socketHandle->m_fd) == 0;
        delete socketHandle;
        return closed;
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
        return [TCS, WriteIO](IOCompletionOverlapped* self, size_t written, int32 err)
        {
            if (err)
            {
                TCS.SetException(std::make_exception_ptr(SystemException(err)));
            }
            else
            {
                TCS.SetResult(written);
            }
        };
    }

    bool LinuxPlatformIO::WriteFile(void* handle, std::span<const uint8> inBytes, IOCompletionOverlapped* overlap) noexcept
    {
        auto* socketHandle = reinterpret_cast<SocketHandle*>(handle);
        auto sqe = socketHandle->m_CompletionPort->get_scoped_sqe();
        io_uring_prep_write(sqe, socketHandle->m_fd, inBytes.data(), inBytes.size_bytes(), (__u64)-1);
        io_uring_sqe_set_data(sqe, overlap);
        return true;
    }

    Action<IOCompletionOverlapped*, size_t, int32> LinuxPlatformIO::FileIOReadAction(TaskCompletionSource<size_t> TCS, void* ReadIO) noexcept
    {
        return [TCS, ReadIO](IOCompletionOverlapped* self, size_t read, int32 err)
        {
            if (err)
            {
                TCS.SetException(std::make_exception_ptr(SystemException(err)));
            }
            else
            {
                TCS.SetResult(read);
            }
        };
    }

    bool LinuxPlatformIO::ReadFile(void* handle, std::span<uint8> outBytes, IOCompletionOverlapped* overlap) noexcept
    {
        auto* socketHandle = reinterpret_cast<SocketHandle*>(handle);
        auto sqe = socketHandle->m_CompletionPort->get_scoped_sqe();
        io_uring_prep_read(sqe, socketHandle->m_fd, outBytes.data(), outBytes.size_bytes(), (__u64)-1);
        io_uring_sqe_set_data(sqe, overlap);
        return true;
    }
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif
