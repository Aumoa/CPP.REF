// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "Net/Sockets/AddressFamily.h"
#include "Net/Sockets/SocketType.h"
#include <memory>

namespace Ayla
{
	class CORE_API Socket
	{
		Socket() = delete;
		Socket(const Socket&) = delete;
		Socket(Socket&&) = delete;
		Socket& operator =(const Socket&) = delete;
		Socket& operator =(Socket&&) = delete;

	private:
		struct PlatformSocket;

	private:
		std::unique_ptr<PlatformSocket> m_Socket;

	public:
		Socket(AddressFamily af, SocketType sf);
		~Socket() noexcept;

		void Close();
	};
}