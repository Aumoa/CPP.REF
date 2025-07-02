#if PLATFORM_LINUX

#include <arpa/inet.h>

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

	struct Socket::PlatformSocket
	{
		const int m_Socket;

		PlatformSocket(AddressFamily af, SocketType st)
			: m_Socket{ socket(AddressFamilyToInt32(af), SocketTypeToInt32(st), 0) }
		{
		}

		~PlatformSocket() noexcept
		{
			close(m_Socket);
		}
	};
}

#endif