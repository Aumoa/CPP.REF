// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "ArgumentException.h"
#include "Net/Sockets/SocketError.h"
#include "Net/Sockets/SocketException.h"

namespace Ayla
{
	static int32 AddressFamilyToInt32(AddressFamily af)
	{
		switch (af)
		{
		case AddressFamily::InterNetwork:
			return AF_INET;
		case AddressFamily::InterNetworkV6:
			return AF_INET6;
		}

		throw ArgumentException(TEXT("af"));
	}

	static AddressFamily Int32ToAddressFamily(int32 af)
	{
		switch (af)
		{
		case AF_INET:
			return AddressFamily::InterNetwork;
		case AF_INET6:
			return AddressFamily::InterNetworkV6;
		}

		throw ArgumentException(TEXT("af"));
	}

	static int32 SocketTypeToInt32(SocketType st)
	{
		switch (st)
		{
		case SocketType::Stream:
			return SOCK_STREAM;
		case SocketType::Dgram:
			return SOCK_DGRAM;
		}

		throw ArgumentException(TEXT("st"));
	}

	static SocketType Int32ToSocketType(int32 st)
	{
		switch (st)
		{
		case SOCK_STREAM:
			return SocketType::Stream;
		case SOCK_DGRAM:
			return SocketType::Dgram;
		}

		throw ArgumentException(TEXT("st"));
	}

	static int32 IPEndPointToSockAddr(const IPEndPoint& endPoint, sockaddr_storage& addr)
	{
		memset(&addr, 0, sizeof(addr));

		if (endPoint.GetAddressFamily() == AddressFamily::InterNetwork)
		{
			auto* addr4 = reinterpret_cast<sockaddr_in*>(&addr);
			addr4->sin_family = AF_INET;
			addr4->sin_port = htons(endPoint.GetPort());

			auto bytes = endPoint.GetAddress().GetAddressBytes();
			memcpy(&addr4->sin_addr, bytes.data(), 4);

			return sizeof(sockaddr_in);
		}
		else
		{
			auto* addr6 = reinterpret_cast<sockaddr_in6*>(&addr);
			addr6->sin6_family = AF_INET6;
			addr6->sin6_port = htons(endPoint.GetPort());

			auto bytes = endPoint.GetAddress().GetAddressBytes();
			memcpy(&addr6->sin6_addr, bytes.data(), 16);

			return sizeof(sockaddr_in6);
		}
	}

	static IPEndPoint SockAddrToIPEndPoint(const sockaddr_storage& addr)
	{
		if (addr.ss_family == AF_INET)
		{
			const auto* addr4 = reinterpret_cast<const sockaddr_in*>(&addr);
			std::array<uint8, 4> bytes;
			memcpy(bytes.data(), &addr4->sin_addr, 4);
			return IPEndPoint(IPAddress(bytes), ntohs(addr4->sin_port));
		}
		else if (addr.ss_family == AF_INET6)
		{
			const auto* addr6 = reinterpret_cast<const sockaddr_in6*>(&addr);
			std::array<uint8, 16> bytes;
			memcpy(bytes.data(), &addr6->sin6_addr, 16);
			return IPEndPoint(IPAddress(bytes), ntohs(addr6->sin6_port));
		}
		else
		{
			throw SocketException(SocketError::AddressFamilyNotSupported);
		}
	}
}
