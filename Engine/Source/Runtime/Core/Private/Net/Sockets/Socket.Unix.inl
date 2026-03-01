// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include <arpa/inet.h>
#include "Socket.Common.inl"

namespace Ayla
{
	struct Socket::PlatformSocket
	{
		const int m_Socket;

		PlatformSocket(AddressFamily af, SocketType st)
			: m_Socket{ socket(AddressFamilyToInt32(af), SocketTypeToInt32(st), 0) }
		{}

		~PlatformSocket() noexcept
		{
			close(m_Socket);
		}
	};
}
