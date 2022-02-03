// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Sockets/Socket.h"
#include "Sockets/SocketException.h"
#include "Net/IPEndPoint.h"

GENERATE_BODY(SSocket);

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
		throw gcnew SSocketException(std::format(L"Unknown address family type({}).", (int32)Family));
	}
}

inline int32 GetRawSocketType(ESocketType SocketType)
{
	switch (SocketType)
	{
	case ESocketType::Stream:
		return SOCK_STREAM;
	default:
		throw gcnew SSocketException(std::format(L"Unknown socket type({}).", (int32)SocketType));
	}
}

inline int32 GetRawProtocolType(EProtocolType ProtocolType)
{
	switch (ProtocolType)
	{
	case EProtocolType::TCP:
		return IPPROTO_TCP;
	default:
		throw gcnew SSocketException(std::format(L"Unknown protocol type({}).", (int32)ProtocolType));
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

SSocket::SSocket(EAddressFamily Family, ESocketType SocketType, EProtocolType ProtocolType) : Super()
{
	Impl = std::make_unique<SocketImpl>(Family, SocketType, ProtocolType);

	int32 RawFamily = GetRawFamily(Family);
	int32 RawSocketType = GetRawSocketType(SocketType);
	int32 RawProtocolType = GetRawProtocolType(ProtocolType);
	Impl->Socket = socket(RawFamily, RawSocketType, RawProtocolType);
	if (Impl->Socket == INVALID_SOCKET)
	{
		throw gcnew SSocketException(std::format(L"Couldn't create new socket."));
	}

	u_long NonBlockingMode = 1;
	if (ioctlsocket(Impl->Socket, FIONBIO, &NonBlockingMode) == SOCKET_ERROR)
	{
		throw gcnew SSocketException(L"Couldn't create non-blocking socket.");
	}
}

SSocket::~SSocket()
{
	ensureMsgf(bClosed, L"Socket does not closed.");
}

Task<> SSocket::Connect(const IPEndPoint& EndPoint)
{
	return Impl->Connect(EndPoint);
}

Task<> SSocket::Send(const void* Buf, size_t Size)
{
	return Impl->Send(Buf, Size);
}

Task<size_t> SSocket::Recv(void* OutBuf, size_t Size, bool bVerifiedLength)
{
	return Impl->Recv(OutBuf, Size, bVerifiedLength);
}

void SSocket::Close()
{
	if (closesocket(Impl->Socket) == SOCKET_ERROR)
	{
		throw gcnew SSocketException(std::format(L"Couldn't close socket. 'closesocket' function return SOCKET_ERROR."));
	}

	bClosed = true;
}

SSocket* SSocket::NewTCPSocket()
{
	return gcnew SSocket(EAddressFamily::InterNetwork, ESocketType::Stream, EProtocolType::TCP);
}

bool SSocket::IsReadyToRead(std::chrono::microseconds Timeout)
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

bool SSocket::IsReadyToWrite(std::chrono::microseconds Timeout)
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