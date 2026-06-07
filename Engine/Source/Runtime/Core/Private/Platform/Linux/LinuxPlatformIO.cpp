// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/Linux/LinuxPlatformIO.h"

#if PLATFORM_LINUX

#define __ALLOW_PLATFORM_COMMON_H__

#include "AssertionMacros.h"
#include "SystemException.h"
#include "Threading/ThreadPool.h"
#include "Platform/PlatformCommon.h"
#include "Platform/Unix/UnixStandardStreamTextWriter.h"
#include "IO/IOCompletionOverlapped.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <cerrno>
#include <limits>
#include <span>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <liburing.h>

namespace Ayla
{
    class IOCompletionPort
    {
    private:
        struct Completion
        {
            IOCompletionOverlapped* m_Overlap = nullptr;
            size_t m_Bytes = 0;
            int32 m_Error = 0;
        };

        static constexpr uint32 kUringQueueDepth = 64;
        io_uring m_Ring;

        std::queue<Completion> m_Overlaps;
        std::mutex m_Mutex;
        std::condition_variable m_Cond;

        std::mutex m_RingMutex;
        std::mutex m_CancellationLock;
        std::stop_source m_DispatchCancel;
        std::thread m_DispatchThread;

    public:
        struct scoped_sqe
        {
            IOCompletionPort* m_CompletionPort = nullptr;
            io_uring_sqe* m_sqe = nullptr;

            scoped_sqe(IOCompletionPort* completionPort)
                : m_CompletionPort{ completionPort }
            {
                completionPort->m_RingMutex.lock();
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
                    if (m_sqe != nullptr)
                    {
                        io_uring_submit(&m_CompletionPort->m_Ring);
                    }
                    m_CompletionPort->m_RingMutex.unlock();
                }
            }

            inline operator io_uring_sqe*() const noexcept { return m_sqe; }
            inline explicit operator bool() const noexcept { return m_sqe != nullptr; }
        };

    public:
        IOCompletionPort()
        {
            check(io_uring_queue_init(static_cast<unsigned int>(kUringQueueDepth), &m_Ring, 0) == 0);
            m_DispatchThread = std::thread([this]() { this->io_uring_dispatch(this->m_DispatchCancel.get_token()); });
        }

        ~IOCompletionPort() noexcept
        {
            RequestStop();
            if (m_DispatchThread.joinable())
            {
                m_DispatchThread.join();
            }
            io_uring_queue_exit(&m_Ring);
        }

        bool DispatchQueuedCompletionStatus(const TimeSpan& dur) noexcept
        {
            auto lock = std::unique_lock{ m_Mutex };
            auto cancellationToken = m_DispatchCancel.get_token();
            if (m_Overlaps.empty())
            {
                auto pred = [this, &cancellationToken]()
                {
                    return m_Overlaps.empty() == false || cancellationToken.stop_requested();
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

            if (cancellationToken.stop_requested())
            {
                return false;
            }

            if (m_Overlaps.empty())
            {
                return false;
            }

            auto completion = m_Overlaps.front();
            m_Overlaps.pop();
            lock.unlock();

            if (completion.m_Error == 0)
            {
                completion.m_Overlap->Complete(completion.m_Bytes);
            }
            else
            {
                completion.m_Overlap->Failed(completion.m_Error);
            }

            return true;
        }

        void RequestStop() noexcept
        {
            m_DispatchCancel.request_stop();
            QueueWakeup();
            m_Cond.notify_all();
        }

        void QueueWakeup() noexcept
        {
            auto lock = std::unique_lock{ m_RingMutex };
            io_uring_sqe* sqe = io_uring_get_sqe(&m_Ring);
            if (sqe != nullptr)
            {
                io_uring_prep_nop(sqe);
                io_uring_sqe_set_data(sqe, nullptr);
                io_uring_submit(&m_Ring);
            }
        }
        
        void io_uring_dispatch(std::stop_token cancellationToken)
        {
            while (true)
            {
                auto lock = std::unique_lock{ m_CancellationLock };
                if (cancellationToken.stop_requested())
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
                    int32 error = cqe->res < 0 ? static_cast<int32>(-cqe->res) : 0;
                    size_t bytes = cqe->res < 0 ? 0 : static_cast<size_t>(cqe->res);
                    io_uring_cqe_seen(&this->m_Ring, cqe);

                    if (overlap == nullptr)
                    {
                        if (cancellationToken.stop_requested())
                        {
                            break;
                        }
                        continue;
                    }

                    auto lock = std::unique_lock{ m_Mutex };
                    m_Overlaps.emplace(Completion
                    {
                        .m_Overlap = overlap,
                        .m_Bytes = bytes,
                        .m_Error = error
                    });
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
        static UnixStandardStreamTextWriter writer(STDOUT_FILENO);
        return writer;
    }

    TextWriter& LinuxPlatformIO::GetStandardError() noexcept
    {
        static UnixStandardStreamTextWriter writer(STDERR_FILENO);
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
        (void)size;
        auto iocp = reinterpret_cast<IOCompletionPort*>(handle);
        iocp->RequestStop();
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
        auto* socketHandle = reinterpret_cast<SocketHandle*>(Handle);
        return fsync(socketHandle->m_fd) == 0;
    }

    bool LinuxPlatformIO::SetFileSeekPointer(void* Handle, int64 Seekpos, SeekOrigin InOrigin) noexcept
    {
        auto* socketHandle = reinterpret_cast<SocketHandle*>(Handle);
        int whence = SEEK_SET;
        switch (InOrigin)
        {
        case SeekOrigin::Begin:   whence = SEEK_SET; break;
        case SeekOrigin::Current: whence = SEEK_CUR; break;
        case SeekOrigin::End:     whence = SEEK_END; break;
        }
        return lseek(socketHandle->m_fd, Seekpos, whence) != -1;
    }

    int64 LinuxPlatformIO::GetFileSize(void* Handle) noexcept
    {
        auto* socketHandle = reinterpret_cast<SocketHandle*>(Handle);
        struct stat st;
        if (fstat(socketHandle->m_fd, &st) == -1)
        {
            return 0;
        }
        return static_cast<int64>(st.st_size);
    }

    MoveOnlyFunction<void(IOCompletionOverlapped*, size_t, int32)> LinuxPlatformIO::FileIOWrittenAction(TaskCompletionSource<size_t> TCS, void* WriteIO) noexcept
    {
        return [TCS, WriteIO](IOCompletionOverlapped* self, size_t written, int32 err)
        {
            (void)self;
            (void)WriteIO;
            if (err)
            {
                TCS.TrySetException<SystemException>(err);
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
        if (!sqe)
        {
            return false;
        }
        io_uring_prep_write(sqe, socketHandle->m_fd, inBytes.data(), inBytes.size_bytes(), (__u64)-1);
        io_uring_sqe_set_data(sqe, overlap);
        return true;
    }

    MoveOnlyFunction<void(IOCompletionOverlapped*, size_t, int32)> LinuxPlatformIO::FileIOReadAction(TaskCompletionSource<size_t> TCS, void* ReadIO) noexcept
    {
        return [TCS, ReadIO](IOCompletionOverlapped* self, size_t read, int32 err)
        {
            (void)self;
            (void)ReadIO;
            if (err)
            {
                TCS.TrySetException<SystemException>(err);
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
        if (!sqe)
        {
            return false;
        }
        io_uring_prep_read(sqe, socketHandle->m_fd, outBytes.data(), outBytes.size_bytes(), (__u64)-1);
        io_uring_sqe_set_data(sqe, overlap);
        return true;
    }
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif
