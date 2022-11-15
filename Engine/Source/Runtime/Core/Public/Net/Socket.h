// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/NonCopyable.h"
#include "Net/AddressFamily.h"
#include "Net/SocketType.h"
#include "Net/ProtocolType.h"
#include "Net/EndPoint.h"

class CORE_API Socket : public NonCopyable
{
	static constexpr size_t INVALID_SOCKET = -1;
	static constexpr int32 SOCKET_ERROR = -1;

	size_t _socket = INVALID_SOCKET;
	std::optional<EndPoint::sockaddr_buf> _ep;

public:
	Socket(EAddressFamily af, ESocketType st, EProtocolType pt);
	~Socket() noexcept;

	void Close() noexcept;
	void Listen(int32 backlog = 0);
	void Bind(const EndPoint& ep);

private:
	static void _trap_init();
	[[noreturn]] static void _throw_error();
};