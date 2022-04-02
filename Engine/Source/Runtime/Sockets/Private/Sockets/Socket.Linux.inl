// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_LINUX

#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

using SOCKET = int;
constexpr int INVALID_SOCKET = -1;

struct Socket::SocketImpl
{
	int Socket = INVALID_SOCKET;
	bool bClosed = false;

	SocketImpl(EAddressFamily Family, ESocketType SocketType, EProtocolType ProtocolType)
	{
		throw not_implemented();
	}

	Task<> Connect(const IPEndPoint& EndPoint)
	{
		throw not_implemented();
	}

	Task<> Send(const void* Buf, size_t Size)
	{
		throw not_implemented();
	}

	Task<size_t> Recv(void* OutBuf, size_t Size, bool bVerifiedLength)
	{
		throw not_implemented();
	}

	void Bind(const IPEndPoint& EndPoint)
	{
		throw not_implemented();
	}
};

#endif