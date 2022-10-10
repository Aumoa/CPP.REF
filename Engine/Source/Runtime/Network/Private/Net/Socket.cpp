// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/Socket.h"
#include "Net/SocketException.h"
#include "NetworkApplication.h"
#include "Threading/Thread.h"
#include "Threading/ThreadGroup.h"
#include "Threading/Tasks/TaskCompletionSource.h"
#include "Socket.gen.cpp"

#if PLATFORM_WINDOWS
#include "WindowsMinimal.h"

class IOCPThreadGroup : public ThreadGroup
{
public:
	IOCPThreadGroup(std::function<void()> invoke)
		: ThreadGroup(TEXT("IOCP"), 1, false, [this, invoke](size_t index, std::stop_token cancellationToken)
		{
			ReadyWorkerThread(index);
			invoke();
		})
	{
	}
};

static std::atomic<size_t> leak;

struct OverlappedEx : public WSAOVERLAPPED
{
	TaskCompletionSource<size_t> tcs;
	bool use;
};

struct SimplyOverlappedPool
{
	std::vector<OverlappedEx> Buffer;
	std::atomic<size_t> Index;

	SimplyOverlappedPool()
		: Buffer(10240)
	{
	}

	OverlappedEx* Alloc()
	{
		while (true)
		{
			OverlappedEx* ptr = Buffer.data() + Index++;
			if (ptr->use == false)
			{
				ptr->use = true;
				return ptr;
			}
		}
	}

	void Free(OverlappedEx* ptr)
	{
		memset((WSAOVERLAPPED*)ptr, 0, sizeof(WSAOVERLAPPED));

		// important! don't change order.
		ptr->tcs = {};
		ptr->use = false;
	}
} pool;

struct Socket::Impl
{
	SOCKET s = INVALID_SOCKET;
	int af;
	std::vector<uint8> addr;
	HANDLE cp = nullptr;

	bool iocpRun = false;
	std::unique_ptr<IOCPThreadGroup> iocpThread;

	Impl()
	{
		static WSAData* __Init_WSA_launch = []()
		{
			static WSAData data;
			if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
			{
				throw SocketException(TEXT("Cannot initialize WSA."));
			}

			return &data;
		}();

		++leak;
	}

	~Impl() noexcept
	{
		--leak;

		if (this->cp)
		{
			check(leak == 1);
		}

		this->s_close();

		if (this->iocpThread.get())
		{
			this->iocpRun = false;
			this->iocpThread.reset();
		}
	}

	void create_socket(int af, int type, int protocol)
	{
		check(s == INVALID_SOCKET);
		this->s = WSASocketW(af, type, protocol, nullptr, 0, WSA_FLAG_OVERLAPPED);
		this->af = af;
		if (this->s == INVALID_SOCKET)
		{
			throw SocketException(TEXT("Invalid arguments detected while create socket."));
		}
	}

	void s_bind(const EndPoint& ep)
	{
		check(this->s != INVALID_SOCKET);

		sockaddr_in addr = {};
		addr.sin_family = af;
		addr.sin_addr.s_addr = htonl((u_long)ep.Address.Long);
		addr.sin_port = htons((u_short)ep.Port);

		if (bind(this->s, (const sockaddr*)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			throw SocketException(TEXT("Cannot bind socket address."));
		}

		this->addr.resize(sizeof(addr));
		memcpy(this->addr.data(), &addr, sizeof(sockaddr_in));
	}

	void s_listen(int32 backlog)
	{
		check(this->s != INVALID_SOCKET);

		if (listen(this->s, backlog ? backlog : SOMAXCONN) == SOCKET_ERROR)
		{
			throw SocketException(TEXT("Cannot ready to listen connections."));
		}

		cp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
		if (cp == nullptr)
		{
			int32 error = WSAGetLastError();
			throw SocketException(String::Format(TEXT("Cannot create IO completion port: {0}"), error));
		}

		iocpRun = true;
		iocpThread = std::make_unique<IOCPThreadGroup>(std::bind(&Impl::IOCPThreadStart, this));
	}

	std::shared_ptr<Impl> s_accept()
	{
		check(this->s != INVALID_SOCKET);

		auto impl = std::make_shared<Impl>();
		impl->addr.resize(sizeof(sockaddr_in));
		int len = (int)impl->addr.size();
		impl->s = WSAAccept(this->s, (sockaddr*)impl->addr.data(), &len, nullptr, 0);
		if (impl->s == INVALID_SOCKET)
		{
			throw SocketException(TEXT("Failed to accept connection."));
		}

		this->cp = CreateIoCompletionPort((HANDLE)impl->s, this->cp, (ULONG_PTR)impl.get(), 0);
		if (this->cp == nullptr)
		{
			throw SocketException(TEXT("Failed to create IOCP handle."));
		}

		return impl;
	}

	Task<size_t> s_recv(void* buf, size_t len, std::stop_token cancellationToken)
	{
		DWORD len_r = 0;
		DWORD flag = 0;

		WSABUF wbuf;
		wbuf.buf = (CHAR*)buf;
		wbuf.len = (ULONG)len;

		auto tcs = TaskCompletionSource<>::Create<size_t>(cancellationToken);
		{
			auto overlapped = pool.Alloc();
			overlapped->tcs = tcs;

			if (WSARecv(this->s, &wbuf, 1, &len_r, &flag, overlapped, nullptr) == SOCKET_ERROR)
			{
				if (int error = WSAGetLastError(); error != ERROR_IO_PENDING)
				{
					throw SocketException(String::Format(TEXT("Cannot receive any buffers from socket. ({0})"), error));
				}
			}
		}

		return tcs.GetTask();
	}

	Task<size_t> s_send(const void* buf, size_t len, std::stop_token cancellationToken)
	{
		DWORD len_s = 0;

		WSABUF wbuf;
		wbuf.buf = (CHAR*)buf;
		wbuf.len = (ULONG)len;

		auto tcs = TaskCompletionSource<>::Create<size_t>(cancellationToken);
		{
			auto overlapped = pool.Alloc();
			overlapped->tcs = tcs;

			if (WSASend(this->s, &wbuf, 1, &len_s, 0, overlapped, nullptr) == SOCKET_ERROR)
			{
				if (int error = WSAGetLastError(); error != ERROR_IO_PENDING)
				{
					throw SocketException(String::Format(TEXT("Cannot receive any buffers from socket. ({0})"), error));
				}
			}
		}

		return tcs.GetTask();
	}

	void s_close()
	{
		if (this->s != INVALID_SOCKET)
		{
			closesocket(this->s);
			this->s = INVALID_SOCKET;
		}

		if (this->cp != nullptr)
		{
			CloseHandle(this->cp);
			this->cp = nullptr;
		}
	}

	void IOCPThreadStart()
	{
		while (this->iocpRun)
		{
			DWORD transfer = 0;
			Impl* sock = nullptr;
			LPOVERLAPPED overlapped;

			if (GetQueuedCompletionStatus(this->cp, &transfer, (PULONG_PTR)&sock, &overlapped, INFINITE) == FALSE)
			{
				continue;
			}

			if (sock == nullptr || transfer == 0)
			{
				continue;
			}

			OverlappedEx* overlappedEx = static_cast<OverlappedEx*>(overlapped);
			overlappedEx->tcs.SetResult(transfer);
			pool.Free(overlappedEx);
		}
	}
};
#endif

Socket::Socket()
{
}

Socket::Socket(EAddressFamily addressFamily, ESocketType socketType, EProtocolType protocolType)
	: _impl(std::make_unique<Impl>())
{
	_impl->create_socket((int)addressFamily, (int)socketType, (int)protocolType);
}

Socket::~Socket() noexcept
{
}

void Socket::Bind(const EndPoint& ep)
{
	_impl->s_bind(ep);
}

void Socket::Listen()
{
	_impl->s_listen(0);
}

void Socket::Listen(int32 backlog)
{
	_impl->s_listen(backlog);
}

std::unique_ptr<Socket> Socket::Accept()
{
	auto s = std::make_unique<Socket>();
	s->_impl = _impl->s_accept();
	return s;
}

Task<size_t> Socket::ReceiveAsync(void* bufferToRecv, size_t len, std::stop_token cancellationToken)
{
	return _impl->s_recv(bufferToRecv, len, cancellationToken);
}

Task<size_t> Socket::SendAsync(const void* bufferToSend, size_t len, std::stop_token cancellationToken)
{
	return _impl->s_send(bufferToSend, len, cancellationToken);
}