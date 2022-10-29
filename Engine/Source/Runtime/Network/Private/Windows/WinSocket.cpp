// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "WinSocket.h"
#include "WindowsIOCP.h"
#include "Net/SocketException.h"

WinSocket::WinSocket()
	: _overlaps(new WinOverlappedContainer())
{
	static auto sIOCP = std::make_unique<WindowsIOCP>();
	_socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	if (_socket == INVALID_SOCKET)
	{
		throw SocketException(FormatWSAError());
	}

	_overlaps->AddRef();
}

WinSocket::~WinSocket() noexcept
{
	if (_overlaps)
	{
		_overlaps->Release();
		_overlaps = nullptr;
	}

	if (_socket != INVALID_SOCKET)
	{
		Close();
	}
}

const void* WinSocket::GetSocket() const noexcept
{
	return reinterpret_cast<const void*>(_socket);
}

void WinSocket::Bind(const IPEndPoint& ep)
{
	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl((u_long)ep.Address.Long);
	addr.sin_port = htons((u_short)ep.Port);

	if (bind(_socket, (const sockaddr*)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		throw SocketException(FormatWSAError());
	}

	IOCompletionPort::Get()->ChainSocket(this);
}

Task<> WinSocket::ConnectAsync(const IPEndPoint& ep)
{
	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = (u_long)ep.Address.Long;
	addr.sin_port = htons((u_short)ep.Port);

	if (WSAConnectEx == nullptr)
	{
		GUID guid = WSAID_CONNECTEX;
		DWORD bytesReturned = 0;
		if (WSAIoctl(_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &WSAConnectEx, sizeof(WSAConnectEx), &bytesReturned, nullptr, nullptr) != 0)
		{
			throw SocketException(FormatWSAError());
		}
	}
	
	Bind(IPEndPoint{});

	WinOverlapped* block = _overlaps->GetRef(EIOCPAsyncOp::Connect);
	DWORD bytesSent = 0;
//	std::cerr << std::format("Connect, Socket: {}\n", (int)_socket);
	if (WSAConnectEx(_socket, (const sockaddr*)&addr, sizeof(addr), nullptr, 0, &bytesSent, block) == FALSE)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			throw SocketException(FormatWSAError(errorCode));
		}

		return block->Signal.GetTask();
	}

	block->Release();
	return Task<>::CompletedTask();
}

void WinSocket::Listen()
{
	BOOL on = TRUE;
	if (setsockopt(_socket, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&on, sizeof(on)) == SOCKET_ERROR)
	{
		throw SocketException(FormatWSAError());
	}

	if (listen(_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw SocketException(FormatWSAError());
	}
}

Task<std::shared_ptr<ImplSocket>> WinSocket::AcceptAsync()
{
	auto sAccept = std::make_shared<WinSocket>();

	if (WSAAcceptEx == nullptr)
	{
		GUID guid = WSAID_ACCEPTEX;
		DWORD bytesReturned = 0;
		if (WSAIoctl(_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &WSAAcceptEx, sizeof(WSAAcceptEx), &bytesReturned, nullptr, nullptr) != 0)
		{
			throw SocketException(FormatWSAError());
		}

		guid = WSAID_GETACCEPTEXSOCKADDRS;
		if (WSAIoctl(_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &WSAGetAcceptExSockaddrs, sizeof(WSAGetAcceptExSockaddrs), &bytesReturned, nullptr, nullptr) != 0)
		{
			throw SocketException(FormatWSAError());
		}
	}

	char buf[(sizeof(SOCKADDR_IN) + 16) * 2];
	DWORD recv = 0;

	WinOverlapped* block = _overlaps->GetRef(EIOCPAsyncOp::Accept);
	if (WSAAcceptEx(_socket, sAccept->_socket, buf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &recv, block) == FALSE)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			throw SocketException(FormatWSAError(errorCode));
		}

		co_await block->Signal.GetTask();
	}

	IOCompletionPort::Get()->ChainSocket(sAccept.get());
	co_return sAccept;
}

Task<size_t> WinSocket::ReceiveAsync(void* bufferToRecv, size_t len, std::stop_token cancellationToken)
{
	DWORD len_r = 0;
	DWORD flag = 0;

	WSABUF wbuf;
	wbuf.buf = (CHAR*)bufferToRecv;
	wbuf.len = (ULONG)len;

	WinOverlapped* block = _overlaps->GetRef(EIOCPAsyncOp::Receive);
	if (WSARecv(_socket, &wbuf, 1, &len_r, &flag, block, nullptr) == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			throw SocketException(FormatWSAError(errorCode));
		}

		return block->Signal.GetTask();
	}

	return Task<>::FromResult((size_t)len_r);
}

Task<size_t> WinSocket::SendAsync(const void* bufferToSend, size_t len, std::stop_token cancellationToken)
{
	DWORD len_r = 0;

	WSABUF wbuf;
	wbuf.buf = (CHAR*)bufferToSend;
	wbuf.len = (ULONG)len;

	WinOverlapped* block = _overlaps->GetRef(EIOCPAsyncOp::Send);
	if (WSASend(_socket, &wbuf, 1, &len_r, 0, block, nullptr) == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			throw SocketException(FormatWSAError(errorCode));
		}

		return block->Signal.GetTask();
	}

	return Task<>::FromResult((size_t)len_r);
}

void WinSocket::Close()
{
	check(_socket != INVALID_SOCKET);
	closesocket(_socket);
	_socket = INVALID_SOCKET;
}

#endif