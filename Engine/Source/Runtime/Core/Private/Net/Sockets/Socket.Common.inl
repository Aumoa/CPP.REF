// Copyright 2020-2025 Aumoa.lib. All right reserved.

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
}