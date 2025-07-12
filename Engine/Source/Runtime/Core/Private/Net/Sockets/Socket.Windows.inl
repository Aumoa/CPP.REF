// Copyright 2020-2025 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/PlatformCommon.h"
#include "Socket.Common.inl"

namespace Ayla
{
	struct Socket::PlatformSocket
	{
		const SOCKET m_Socket;

		PlatformSocket(AddressFamily af, SocketType st)
			: m_Socket{ socket(AddressFamilyToInt32(af), SocketTypeToInt32(st), 0) }
		{
		}

		~PlatformSocket() noexcept
		{
			closesocket(m_Socket);
		}
	};
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif