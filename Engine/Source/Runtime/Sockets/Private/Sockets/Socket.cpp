// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Sockets/Socket.h"
#include "Sockets/SocketException.h"
#include "Net/IPEndPoint.h"
#include "CoreAssert.h"

using namespace ::libty;
using namespace ::libty::Sockets;

#include "Socket.Windows.inl"
#include "Socket.Linux.inl"

inline int GetRawFamily(EAddressFamily Family)
{
	switch (Family)
	{
	case EAddressFamily::InterNetwork:
		return AF_INET;
	default:
		throw socket_exception(String::Format("Unknown address family type({}).", (int32)Family));
	}
}

inline int GetRawSocketType(ESocketType SocketType)
{
	switch (SocketType)
	{
	case ESocketType::Stream:
		return SOCK_STREAM;
	default:
		throw socket_exception(String::Format("Unknown socket type({}).", (int32)SocketType));
	}
}

inline int GetRawProtocolType(EProtocolType ProtocolType)
{
	switch (ProtocolType)
	{
	case EProtocolType::TCP:
		return IPPROTO_TCP;
	default:
		throw socket_exception(String::Format("Unknown protocol type({}).", (int32)ProtocolType));
	}
}

inline timeval MakeTimeval(std::chrono::microseconds Value)
{
	using namespace std::chrono;

	seconds Seconds = floor<seconds>(Value);
	Value -= Seconds;

	timeval Timeval = { (long)Seconds.count(), (long)Value.count() };
	return Timeval;
}

inline sockaddr_in GetSOCKADDR_IN(const IPEndPoint& EndPoint)
{
	sockaddr_in InAddr = {};
	InAddr.sin_family = AF_INET;
	InAddr.sin_addr.s_addr = SNetUtility::HostToNetwork(EndPoint.GetAddress());
	InAddr.sin_port = SNetUtility::HostToNetwork(EndPoint.GetPort());
	return InAddr;
}

SSocket::SSocket()
	: Super()
{
}

SSocket::SSocket(EAddressFamily Family, ESocketType SocketType, EProtocolType ProtocolType)
	: Super()
{
	Impl = std::make_unique<SocketImpl>();
	Impl->Socket = socket(GetRawFamily(Family), GetRawSocketType(SocketType), GetRawProtocolType(ProtocolType));

	if (Impl->Socket == INVALID_SOCKET)
	{
		Impl->AbortWithError();
	}
}

SSocket::~SSocket()
{
	if (!GC.IsTearingDown() && IsValid() && !ensure(Impl->bClosed))
	{
		Close();
	}
}

bool SSocket::IsValid() const
{
	return (bool)Impl && !Impl->bClosed;
}

void SSocket::Bind(const IPEndPoint& endpoint)
{
	sockaddr_in sAddr = GetSOCKADDR_IN(endpoint);
	int code = bind(Impl->Socket, (const sockaddr*)&sAddr, sizeof(sAddr));
	if (code == SOCKET_ERROR)
	{
		Impl->AbortWithError();
	}
}

void SSocket::Close()
{
	Impl->Close();
}

void SSocket::Listen(int32 backlog)
{
	if (listen(Impl->Socket, backlog) == SOCKET_ERROR)
	{
		Impl->AbortWithError();
	}
}

SSocket* SSocket::Accept()
{
	sockaddr addr;
	socklen_t szAddr = sizeof(addr);

	SOCKET sock = accept(Impl->Socket, &addr, &szAddr);
	if (sock == INVALID_SOCKET)
	{
		Impl->AbortWithError();
	}

	SSocket* client = gcnew SSocket();
	client->Impl = std::make_unique<SocketImpl>();
	client->Impl->Socket = sock;

	return client;
}

Task<size_t> SSocket::Recv(void* buf, size_t len, ESocketFlags flags)
{
	size_t total = 0;

	while (len > 0)
	{
		u_long available = 0;
		if (ioctlsocket(Impl->Socket, FIONREAD, &available) == SOCKET_ERROR)
		{
			Impl->AbortWithError();
		}

		if (available == 0)
		{
			break;
		}

		size_t rb = len > 0x7FFFFFFF ? 0x7FFFFFFF : len;
		int sz = recv(Impl->Socket, (ReadbackBuf)buf, (int)rb, (int)flags);
		if (sz == 0)
		{
			break;
		}

		len -= sz;
		buf = (char*)buf + sz;
		total += sz;
	}

	co_return total;
}