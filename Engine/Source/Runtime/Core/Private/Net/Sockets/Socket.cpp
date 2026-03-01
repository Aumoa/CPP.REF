// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Net/Sockets/Socket.h"
#include "ArgumentException.h"

#if PLATFORM_LINUX || PLATFORM_OSX
#include "Socket.Unix.inl"
#elif PLATFORM_WINDOWS
#include "Socket.Windows.inl"
#endif

namespace Ayla
{
	Socket::Socket(AddressFamily af, SocketType sf)
		: m_Socket{ std::make_unique<PlatformSocket>(af, sf) }
	{
	}

	Socket::~Socket() noexcept
	{
	}

	void Socket::Close()
	{
		m_Socket.reset();
	}
}