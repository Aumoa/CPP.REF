// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/OSX/OSXPlatformIO.h"

#if PLATFORM_OSX

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
#include <sys/event.h>
#include <cstring>
#include <cerrno>
#include <limits>
#include <span>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

namespace Ayla
{
    class IOCompletionPort
    {
    private:
        enum class FileOperation
        {
            Read,
            Write
        };

        struct Completion
        {
            IOCompletionOverlapped* m_Overlap = nullptr;
            size_t m_Bytes = 0;
            int32 m_Error = 0;
        };

        struct FileRequest
        {
            FileOperation m_Operation = FileOperation::Read;
            int m_Fd = -1;
            const uint8* m_WriteData = nullptr;
            uint8* m_ReadData = nullptr;
            size_t m_Size = 0;
            IOCompletionOverlapped* m_Overlap = nullptr;
        };

        int m_Kqueue;

        std::queue<Completion> m_Overlaps;
        std::mutex m_Mutex;
        std::condition_variable m_Cond;

        std::queue<FileRequest> m_FileRequests;
        std::mutex m_FileMutex;
        std::condition_variable m_FileCond;
        std::vector<std::thread> m_FileWorkers;

        std::stop_source m_DispatchCancel;

    public:
        IOCompletionPort()
        {
            m_Kqueue = kqueue();
            constexpr size_t kFileWorkerCount = 2;
            for (size_t i = 0; i < kFileWorkerCount; ++i)
            {
                m_FileWorkers.emplace_back([this]() { FileWorker(this->m_DispatchCancel.get_token()); });
            }
        }

        ~IOCompletionPort() noexcept
        {
            RequestStop();
            for (auto& worker : m_FileWorkers)
            {
                if (worker.joinable())
                {
                    worker.join();
                }
            }

            if (m_Kqueue >= 0)
            {
                close(m_Kqueue);
            }
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
                if (len == std::chrono::nanoseconds(0))
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
            m_Cond.notify_all();
            m_FileCond.notify_all();
        }

        void QueueCompletion(IOCompletionOverlapped* overlap, size_t result) noexcept
        {
            auto lock = std::unique_lock{ m_Mutex };
            m_Overlaps.emplace(Completion
            {
                .m_Overlap = overlap,
                .m_Bytes = result,
                .m_Error = 0
            });
            m_Cond.notify_one();
        }

        void QueueFailure(IOCompletionOverlapped* overlap, int32 err) noexcept
        {
            auto lock = std::unique_lock{ m_Mutex };
            m_Overlaps.emplace(Completion
            {
                .m_Overlap = overlap,
                .m_Bytes = 0,
                .m_Error = err
            });
            m_Cond.notify_one();
        }

        bool QueueFileRead(int fd, std::span<uint8> outBytes, IOCompletionOverlapped* overlap) noexcept
        {
            return QueueFileRequest(FileRequest
            {
                .m_Operation = FileOperation::Read,
                .m_Fd = fd,
                .m_ReadData = outBytes.data(),
                .m_Size = outBytes.size_bytes(),
                .m_Overlap = overlap
            });
        }

        bool QueueFileWrite(int fd, std::span<const uint8> inBytes, IOCompletionOverlapped* overlap) noexcept
        {
            return QueueFileRequest(FileRequest
            {
                .m_Operation = FileOperation::Write,
                .m_Fd = fd,
                .m_WriteData = inBytes.data(),
                .m_Size = inBytes.size_bytes(),
                .m_Overlap = overlap
            });
        }

    private:
        bool QueueFileRequest(FileRequest request) noexcept
        {
            if (m_DispatchCancel.stop_requested())
            {
                return false;
            }

            auto lock = std::unique_lock{ m_FileMutex };
            m_FileRequests.emplace(request);
            lock.unlock();
            m_FileCond.notify_one();
            return true;
        }

        void FileWorker(std::stop_token cancellationToken) noexcept
        {
            while (true)
            {
                auto lock = std::unique_lock{ m_FileMutex };
                m_FileCond.wait(lock, [this, &cancellationToken]()
                {
                    return !m_FileRequests.empty() || cancellationToken.stop_requested();
                });

                if (m_FileRequests.empty())
                {
                    if (cancellationToken.stop_requested())
                    {
                        break;
                    }
                    continue;
                }

                FileRequest request = m_FileRequests.front();
                m_FileRequests.pop();
                lock.unlock();

                ssize_t result = request.m_Operation == FileOperation::Read
                    ? read(request.m_Fd, request.m_ReadData, request.m_Size)
                    : write(request.m_Fd, request.m_WriteData, request.m_Size);

                if (result >= 0)
                {
                    QueueCompletion(request.m_Overlap, static_cast<size_t>(result));
                }
                else
                {
                    QueueFailure(request.m_Overlap, errno);
                }
            }
        }
    };

    struct SocketHandle
    {
        int m_fd;
        IOCompletionPort* m_CompletionPort;
    };

    TextWriter& OSXPlatformIO::GetStandardOutput() noexcept
    {
        static UnixStandardStreamTextWriter writer(STDOUT_FILENO);
        return writer;
    }

    TextWriter& OSXPlatformIO::GetStandardError() noexcept
    {
        static UnixStandardStreamTextWriter writer(STDERR_FILENO);
        return writer;
    }

    void OSXPlatformIO::InitializeIOCPHandle(void*& Handle) noexcept
    {
        auto* completionPort = new IOCompletionPort();
        Handle = completionPort;
    }

    void OSXPlatformIO::DestroyIOCPHandle(void* Handle) noexcept
    {
        delete reinterpret_cast<IOCompletionPort*>(Handle);
    }

    void OSXPlatformIO::BindIOHandle(void* Handle, void* Socket) noexcept
    {
        auto* socketHandle = reinterpret_cast<SocketHandle*>(Socket);
        socketHandle->m_CompletionPort = reinterpret_cast<IOCompletionPort*>(Handle);
    }

    void OSXPlatformIO::UnbindIOHandle(void* Handle, void* Socket) noexcept
    {
        auto* socketHandle = reinterpret_cast<SocketHandle*>(Socket);
        check(socketHandle->m_CompletionPort == Handle);
        socketHandle->m_CompletionPort = nullptr;
    }

    bool OSXPlatformIO::DispatchQueuedCompletionStatus(void* Handle, const TimeSpan& Dur) noexcept
    {
        auto iocp = reinterpret_cast<IOCompletionPort*>(Handle);
        return iocp->DispatchQueuedCompletionStatus(Dur);
    }

    bool OSXPlatformIO::DispatchQueuedCompletionStatus(void* Handle) noexcept
    {
        return DispatchQueuedCompletionStatus(Handle, TimeSpan::FromMilliseconds(0));
    }

    void OSXPlatformIO::QueueInterruptSignal(void* handle, int32 size) noexcept
    {
        (void)size;
        auto iocp = reinterpret_cast<IOCompletionPort*>(handle);
        iocp->RequestStop();
    }

    void OSXPlatformIO::OpenFileHandle(void*& Handle, String InFilename, FileMode InFileMode, FileAccessMode InAccessMode, FileSharedMode InSharedMode) noexcept
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
        case (FileAccessMode)((uint32)FileAccessMode::Read | (uint32)FileAccessMode::Write):
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

    bool OSXPlatformIO::CloseFileHandle(void* Handle) noexcept
    {
        auto socketHandle = reinterpret_cast<SocketHandle*>(Handle);
        ThreadPool::UnbindHandle(socketHandle);
        bool closed = close(socketHandle->m_fd) == 0;
        delete socketHandle;
        return closed;
    }

    bool OSXPlatformIO::FlushFileBuffers(void* Handle) noexcept
    {
        auto* socketHandle = reinterpret_cast<SocketHandle*>(Handle);
        return fsync(socketHandle->m_fd) == 0;
    }

    bool OSXPlatformIO::SetFileSeekPointer(void* Handle, int64 Seekpos, SeekOrigin InOrigin) noexcept
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

    int64 OSXPlatformIO::GetFileSize(void* Handle) noexcept
    {
        auto* socketHandle = reinterpret_cast<SocketHandle*>(Handle);
        struct stat st;
        if (fstat(socketHandle->m_fd, &st) == -1)
        {
            return 0;
        }
        return static_cast<int64>(st.st_size);
    }

    MoveOnlyFunction<void(IOCompletionOverlapped*, size_t, int32)> OSXPlatformIO::FileIOWrittenAction(TaskCompletionSource<size_t> TCS, void* WriteIO) noexcept
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

    bool OSXPlatformIO::WriteFile(void* handle, std::span<const uint8> inBytes, IOCompletionOverlapped* overlap) noexcept
    {
        auto* socketHandle = reinterpret_cast<SocketHandle*>(handle);
        return socketHandle->m_CompletionPort->QueueFileWrite(socketHandle->m_fd, inBytes, overlap);
    }

    MoveOnlyFunction<void(IOCompletionOverlapped*, size_t, int32)> OSXPlatformIO::FileIOReadAction(TaskCompletionSource<size_t> TCS, void* ReadIO) noexcept
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

    bool OSXPlatformIO::ReadFile(void* handle, std::span<uint8> outBytes, IOCompletionOverlapped* overlap) noexcept
    {
        auto* socketHandle = reinterpret_cast<SocketHandle*>(handle);
        return socketHandle->m_CompletionPort->QueueFileRead(socketHandle->m_fd, outBytes, overlap);
    }
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif
