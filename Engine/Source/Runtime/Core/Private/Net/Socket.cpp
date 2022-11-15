// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/Socket.h"
#include "Exceptions/SocketException.h"
#include "Threading/ThreadPool.h"

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
	ThreadPool::BindHandle((void*)_socket);

	if (listen(_socket, backlog == 0 ? SOMAXCONN : 0) == SOCKET_ERROR)
	{
		_throw_error();
	}
}

void Socket::Bind(const EndPoint& ep)
{
	auto& buf = _ep.emplace();
	ep.ApplyTo(buf);

	if (bind(_socket, (const sockaddr*)&buf, (int)ep.Size()) == SOCKET_ERROR)
	{
		_throw_error();
	}
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
}

[[noreturn]] void Socket::_throw_error()
{
#if PLATFORM_WINDOWS
	throw SocketException(WSAGetLastError());
#endif
}