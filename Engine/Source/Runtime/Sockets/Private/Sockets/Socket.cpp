// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Sockets/Socket.h"
#include "Sockets/SocketException.h"
#include "Net/IPEndPoint.h"
#include "CoreAssert.h"

#if PLATFORM_WINDOWS
#include "Socket.Windows.inl"
#endif

inline int32 GetRawFamily(EAddressFamily Family)
{
	switch (Family)
	{
	case EAddressFamily::InterNetwork:
		return AF_INET;
	default:
		throw socket_exception(std::format("Unknown address family type({}).", (int32)Family));
	}
}

inline int32 GetRawSocketType(ESocketType SocketType)
{
	switch (SocketType)
	{
	case ESocketType::Stream:
		return SOCK_STREAM;
	default:
		throw socket_exception(std::format("Unknown socket type({}).", (int32)SocketType));
	}
}

inline int32 GetRawProtocolType(EProtocolType ProtocolType)
{
	switch (ProtocolType)
	{
	case EProtocolType::TCP:
		return IPPROTO_TCP;
	default:
		throw socket_exception(std::format("Unknown protocol type({}).", (int32)ProtocolType));
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

Socket::Socket(EAddressFamily Family, ESocketType SocketType, EProtocolType ProtocolType)
{
	Impl = std::make_unique<SocketImpl>(Family, SocketType, ProtocolType);

	int32 RawFamily = GetRawFamily(Family);
	int32 RawSocketType = GetRawSocketType(SocketType);
	int32 RawProtocolType = GetRawProtocolType(ProtocolType);
	Impl->Socket = socket(RawFamily, RawSocketType, RawProtocolType);
	if (Impl->Socket == INVALID_SOCKET)
	{
		throw socket_exception("Couldn't create new socket.");
	}

	u_long NonBlockingMode = 1;
	if (ioctlsocket(Impl->Socket, FIONBIO, &NonBlockingMode) == SOCKET_ERROR)
	{
		throw socket_exception("Couldn't create non-blocking socket.");
	}
}

Socket::~Socket()
{
	if (IsValid() && !Impl->bClosed)
	{
		Close();
	}
}

bool Socket::IsValid() const
{
	return (bool)Impl;
}

Task<> Socket::Connect(const IPEndPoint& EndPoint)
{
	return Impl->Connect(EndPoint);
}

Task<> Socket::Send(const void* Buf, size_t Size)
{
	return Impl->Send(Buf, Size);
}

Task<size_t> Socket::Recv(void* OutBuf, size_t Size, bool bVerifiedLength)
{
	return Impl->Recv(OutBuf, Size, bVerifiedLength);
}

void Socket::Bind(const IPEndPoint& endpoint)
{
	return Impl->Bind(endpoint);
}

void Socket::Close()
{
	if (closesocket(Impl->Socket) == SOCKET_ERROR)
	{
		throw socket_exception("Couldn't close socket. 'closesocket' function return SOCKET_ERROR.");
	}

	Impl->bClosed = true;
}

Socket Socket::NewTCPSocket()
{
	return Socket(EAddressFamily::InterNetwork, ESocketType::Stream, EProtocolType::TCP);
}

bool Socket::IsReadyToRead(std::chrono::microseconds Timeout)
{
	using namespace std::chrono;

	fd_set ReadSet;
	FD_ZERO(&ReadSet);

	FD_SET(Impl->Socket, &ReadSet);

	timeval Timeval = MakeTimeval(Timeout);
	int32 SocketAvailable = select(0, &ReadSet, nullptr, nullptr, &Timeval);

	if (SocketAvailable == SOCKET_ERROR)
	{
		//AbortWithError(errno, L"IsReadyToRead()");
		return true;
	}

	return SocketAvailable > 0 && FD_ISSET(Impl->Socket, &ReadSet) != 0;
}

bool Socket::IsReadyToWrite(std::chrono::microseconds Timeout)
{
	using namespace std::chrono;

	fd_set WriteSet;
	FD_ZERO(&WriteSet);

	FD_SET(Impl->Socket, &WriteSet);

	timeval Timeval = MakeTimeval(Timeout);
	int32 SocketAvailable = select(0, nullptr, &WriteSet, nullptr, &Timeval);

	if (SocketAvailable == SOCKET_ERROR)
	{
		//AbortWithError(errno, L"IsReadyToWrite()");
		return true;
	}

	return SocketAvailable > 0 && FD_ISSET(Impl->Socket, &WriteSet) != 0;
}