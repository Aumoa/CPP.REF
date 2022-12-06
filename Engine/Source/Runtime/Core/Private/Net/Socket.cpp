// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/Socket.h"
#include "Exceptions/SocketException.h"
#include "Threading/ThreadPool.h"
#include "Threading/Tasks/TaskCompletionSource.h"
#include "IO/IOCompletionOverlapped.h"

#if PLATFORM_WINDOWS
#include "Misc/WindowsPlatformMisc.h"
#endif

Socket::Socket(EAddressFamily af, ESocketType st, EProtocolType pt)
{
	static int s = (_trap_init(), 0);

#if PLATFORM_WINDOWS
	_socket = WSASocketW((int)af, (int)st, (int)pt, NULL, 0, WSA_FLAG_OVERLAPPED);
#endif
	if (_socket == INVALID_SOCKET)
	{
		_throw_error();
	}
}

Socket::~Socket() noexcept
{
	if (_socket != INVALID_SOCKET)
	{
		Close();
	}
}

void Socket::Close() noexcept
{
	if (_socket)
	{
		closesocket(_socket);
		_socket = INVALID_SOCKET;
	}
}

void Socket::Listen(int32 backlog)
{
	if (listen(_socket, backlog == 0 ? SOMAXCONN : 0) == SOCKET_ERROR)
	{
		_throw_error();
	}
}

void Socket::Bind(const EndPoint& ep)
{
	ThreadPool::BindHandle((void*)_socket);

	EndPoint::sockaddr_buf buf;
	ep.ApplyTo(buf);
	_local.Accept(buf);
	_remote.Accept(buf);

	if (bind(_socket, (const sockaddr*)&buf, (int)ep.Size()) == SOCKET_ERROR)
	{
		_throw_error();
	}
}

Task<> Socket::AcceptAsync(Socket& sock, std::stop_token cancellationToken)
{
#if PLATFORM_WINDOWS
	static LPFN_ACCEPTEX WSAAcceptEx = [this]()
	{
		GUID id = WSAID_ACCEPTEX;
		DWORD bytesReturned = 0;
		LPFN_ACCEPTEX fn = nullptr;
		if (WSAIoctl(_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &id, sizeof(id), &fn, sizeof(fn), &bytesReturned, nullptr, nullptr) == SOCKET_ERROR)
		{
			throw SocketException(WSAGetLastError());
		}

		return fn;
	}();

	static LPFN_GETACCEPTEXSOCKADDRS WSAGetAcceptExSockaddrs = [this]()
	{
		GUID id = WSAID_GETACCEPTEXSOCKADDRS;
		DWORD bytesReturned = 0;
		LPFN_GETACCEPTEXSOCKADDRS fn = nullptr;
		if (WSAIoctl(_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &id, sizeof(id), &fn, sizeof(fn), &bytesReturned, nullptr, nullptr) == SOCKET_ERROR)
		{
			throw SocketException(WSAGetLastError());
		}

		return fn;
	}();

	struct buf
	{
		EndPoint::sockaddr_buf sLocalBuf;
		EndPoint::sockaddr_buf sRemoteBuf;
	};

	auto tcs = TaskCompletionSource<>::Create(cancellationToken);
	auto* ptr = new IOCompletionOverlapped([tcs](size_t, int32 error) mutable
	{
		if (error)
		{
			tcs.SetException(SocketException(error));
		}
		else
		{
			tcs.SetResult();
		}
	});

	auto* overlapped = (LPOVERLAPPED)ptr->ToOverlapped();
	buf b = {};

	DWORD recv;
	if (WSAAcceptEx(_socket, sock._socket, &b, 0, sizeof(b.sLocalBuf), sizeof(b.sRemoteBuf), &recv, overlapped) == FALSE)
	{
		int err = WSAGetLastError();
		if (err != WSA_IO_PENDING)
		{
			ptr->Failed(err);
			delete ptr;
		}

		co_await tcs.GetTask();
	}

	sockaddr* localEp, *remoteEp;
	INT localEpSz, remoteEpSz;
	WSAGetAcceptExSockaddrs(&b, 0, sizeof(b.sLocalBuf), sizeof(b.sRemoteBuf), &localEp, &localEpSz, &remoteEp, &remoteEpSz);

	sock._local.Accept(b.sLocalBuf);
	sock._remote.Accept(b.sRemoteBuf);
#endif
}

Task<> Socket::ConnectAsync(const EndPoint& ep, std::stop_token cancellationToken)
{
	std::error_code ec = co_await TryConnectAsync(ep, cancellationToken);
	if (ec)
	{
		throw SocketException(ec.value());
	}
}

Task<std::error_code> Socket::TryConnectAsync(const EndPoint& ep, std::stop_token cancellationToken)
{
	EndPoint::sockaddr_buf sBuf;
	ep.ApplyTo(sBuf);

#if PLATFORM_WINDOWS
	static LPFN_CONNECTEX WSAConnectEx = [this]()
	{
		GUID id = WSAID_CONNECTEX;
		DWORD bytesReturned = 0;
		LPFN_CONNECTEX fn = nullptr;
		if (WSAIoctl(_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &id, sizeof(id), &fn, sizeof(fn), &bytesReturned, nullptr, nullptr) == SOCKET_ERROR)
		{
			throw SocketException(WSAGetLastError());
		}

		return fn;
	}();

	auto tcs = TaskCompletionSource<>::Create<std::error_code>(cancellationToken);
	auto* ptr = new IOCompletionOverlapped([tcs](size_t, int32 error) mutable
	{
		if (error)
		{
			tcs.SetResult(std::error_code(error, std::system_category()));
		}
		else
		{
			tcs.SetResult(std::error_code());
		}
	});

	std::error_code ec;
	if (WSAConnectEx(_socket, reinterpret_cast<const sockaddr*>(&sBuf), (int)ep.Size(), NULL, 0, NULL, reinterpret_cast<LPOVERLAPPED>(ptr->ToOverlapped())) == FALSE)
	{
		int err = WSAGetLastError();
		if (err != WSA_IO_PENDING)
		{
			ptr->Failed(err);
			delete ptr;
		}

		ec = co_await tcs.GetTask();
	}
#endif

	_remote.Accept(sBuf);
	co_return ec;
}

Task<size_t> Socket::SendAsync(std::span<const uint8> bytesToSend, std::stop_token cancellationToken)
{
#if PLATFORM_WINDOWS
	DWORD cur;
	auto tcs = TaskCompletionSource<>::Create<size_t>(cancellationToken);
	auto* ptr = new IOCompletionOverlapped([tcs](size_t resolved, int32 error) mutable
	{
		if (error)
		{
			tcs.SetException(SocketException(error));
		}
		else
		{
			tcs.SetResult(resolved);
		}
	});

	WSABUF wbuf;
	wbuf.buf = reinterpret_cast<CHAR*>(const_cast<uint8*>(bytesToSend.data()));
	wbuf.len = static_cast<DWORD>(bytesToSend.size_bytes());
	int r = WSASend(_socket, &wbuf, 1, &cur, 0, reinterpret_cast<LPWSAOVERLAPPED>(ptr->ToOverlapped()), NULL);
	if (r == -1)
	{
		if (int err = WSAGetLastError(); err != WSA_IO_PENDING)
		{
			ptr->Failed(err);
			delete ptr;
		}

		cur = (DWORD)co_await tcs.GetTask();
	}

	co_return (size_t)cur;
#endif
}

Task<> Socket::SendToAsync(const EndPoint& ep, std::span<const uint8> bytesToSend, std::stop_token cancellationToken)
{
#if PLATFORM_WINDOWS
	DWORD cur;
	DWORD flags = 0;
	auto tcs = TaskCompletionSource<>::Create<size_t>(cancellationToken);
	auto* ptr = new IOCompletionOverlapped([tcs](size_t resolved, int32 error) mutable
	{
		if (error)
		{
			tcs.SetException(SocketException(error));
		}
		else
		{
			tcs.SetResult(resolved);
		}
	});

	EndPoint::sockaddr_buf buf;
	ep.ApplyTo(buf);

	WSABUF wbuf;
	wbuf.buf = reinterpret_cast<CHAR*>(const_cast<uint8*>(bytesToSend.data()));
	wbuf.len = static_cast<DWORD>(bytesToSend.size_bytes());
	int s = WSASendTo(_socket, &wbuf, 1, &cur, flags, reinterpret_cast<const sockaddr*>(&buf), (int)ep.Size(), reinterpret_cast<LPWSAOVERLAPPED>(ptr->ToOverlapped()), NULL);
	if (s == -1)
	{
		if (int err = WSAGetLastError(); err != WSA_IO_PENDING)
		{
			ptr->Failed(err);
			delete ptr;
		}

		s = (int)co_await tcs.GetTask();
	}
#endif
}

Task<size_t> Socket::ReceiveAsync(std::span<uint8> bytesToReceive, std::stop_token cancellationToken)
{
#if PLATFORM_WINDOWS
	DWORD cur;
	DWORD flags = 0;
	auto tcs = TaskCompletionSource<>::Create<size_t>(cancellationToken);
	auto* ptr = new IOCompletionOverlapped([tcs](size_t resolved, int32 error) mutable
	{
		if (error)
		{
			tcs.SetException(SocketException(error));
		}
		else
		{
			tcs.SetResult(resolved);
		}
	});

	WSABUF wbuf;
	wbuf.buf = reinterpret_cast<CHAR*>(bytesToReceive.data());
	wbuf.len = static_cast<DWORD>(bytesToReceive.size_bytes());
	int r = WSARecv(_socket, &wbuf, 1, &cur, &flags, reinterpret_cast<LPWSAOVERLAPPED>(ptr->ToOverlapped()), NULL);
	if (r == -1)
	{
		if (int err = WSAGetLastError(); err != WSA_IO_PENDING)
		{
			ptr->Failed(err);
			delete ptr;
		}

		cur = (DWORD)co_await tcs.GetTask();
	}

	co_return (size_t)cur;
#endif
}

Task<> Socket::ReceiveFromAsync(EndPoint& ep, std::span<uint8> bytesToReceive, std::stop_token cancellationToken)
{
#if PLATFORM_WINDOWS
	DWORD cur;
	DWORD flags = 0;
	auto tcs = TaskCompletionSource<>::Create<size_t>(cancellationToken);
	auto* ptr = new IOCompletionOverlapped([tcs](size_t resolved, int32 error) mutable
	{
		if (error)
		{
			tcs.SetException(SocketException(error));
		}
		else
		{
			tcs.SetResult(resolved);
		}
	});

	WSABUF wbuf;
	wbuf.buf = reinterpret_cast<CHAR*>(bytesToReceive.data());
	wbuf.len = static_cast<DWORD>(bytesToReceive.size_bytes());

	EndPoint::sockaddr_buf buf;
	INT sz = sizeof(buf);
	int r = WSARecvFrom(_socket, &wbuf, 1, &cur, &flags, reinterpret_cast<sockaddr*>(&buf), &sz, reinterpret_cast<LPWSAOVERLAPPED>(ptr->ToOverlapped()), NULL);
	if (r == -1)
	{
		if (int err = WSAGetLastError(); err != WSA_IO_PENDING)
		{
			ptr->Failed(err);
			delete ptr;
		}

		r = (int)co_await tcs.GetTask();
	}

	ep.Accept(buf);
#endif
}

void Socket::_trap_init()
{
#if PLATFORM_WINDOWS
	static WSAData ws;
#endif

	static int s = []()
	{
#if PLATFORM_WINDOWS
		if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
		{
			_throw_error();
		}
#endif

		return 0;
	}();

	static struct _trap_shutdown
	{
		~_trap_shutdown() noexcept
		{
#if PLATFORM_WINDOWS
			WSACleanup();
#endif
		}
	} _ts;
}

[[noreturn]] void Socket::_throw_error()
{
#if PLATFORM_WINDOWS
	throw SocketException(WSAGetLastError());
#endif
}