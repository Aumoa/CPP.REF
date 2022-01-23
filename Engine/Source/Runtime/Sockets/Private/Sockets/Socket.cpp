// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Sockets/Socket.h"
#include "Sockets/SocketException.h"
#include "Net/IPEndPoint.h"
#include "Threading/DeferredTask.h"
#include <mutex>

GENERATE_BODY(SSocket);

#include "Socket.Windows.inl"

inline SOCKADDR_IN GetSOCKADDR_IN(const IPEndPoint& EndPoint)
{
	SOCKADDR_IN InAddr = {};
	InAddr.sin_family = AF_INET;
	InAddr.sin_addr.s_addr = EndPoint.IP.Address;
	InAddr.sin_port = EndPoint.GetNetPort();
	return InAddr;
}

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

Task<void> SSocket::Connect(const IPEndPoint& EndPoint)
{
	auto InAddr = GetSOCKADDR_IN(EndPoint);
	if (connect(Impl->Socket, (SOCKADDR*)&InAddr, sizeof(InAddr)) != SOCKET_ERROR)
	{
		co_return;
	}

	while (!IsReadyToRead(std::chrono::milliseconds(1)))
	{
		co_await DeferredTask<void>::YieldTask();
	}
}

Task<void> SSocket::Bind(const IPEndPoint& EndPoint)
{
	auto InAddr = GetSOCKADDR_IN(EndPoint);
	if (bind(Impl->Socket, (SOCKADDR*)&InAddr, sizeof(InAddr)) != SOCKET_ERROR)
	{
		co_return;
	}

	while (!IsReadyToRead(std::chrono::milliseconds(1)))
	{
		co_await DeferredTask<void>::YieldTask();
	}
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

bool SSocket::IsReadyToRead(std::chrono::microseconds Timeout) const
{
	using namespace std::chrono;

	fd_set ReadSet;
	FD_ZERO(&ReadSet);

	FD_SET(Impl->Socket, &ReadSet);

	timeval Timeval = MakeTimeval(Timeout);
	int32 SocketAvailable = select(0, &ReadSet, nullptr, nullptr, &Timeval);

	return SocketAvailable > 0 && FD_ISSET(Impl->Socket, &ReadSet) != 0;
}

bool SSocket::IsReadyToWrite(std::chrono::microseconds Timeout) const
{
	using namespace std::chrono;

	fd_set WriteSet;
	FD_ZERO(&WriteSet);

	FD_SET(Impl->Socket, &WriteSet);

	timeval Timeval = MakeTimeval(Timeout);
	int32 SocketAvailable = select(0, nullptr, &WriteSet, nullptr, &Timeval);

	return SocketAvailable > 0 && FD_ISSET(Impl->Socket, &WriteSet) != 0;
}