// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Net/Sockets/Socket.h"
#include "ArgumentException.h"

#include "Socket.Linux.inl"
#include "Socket.Windows.inl"

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