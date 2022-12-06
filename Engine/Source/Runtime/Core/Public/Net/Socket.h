// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/NonCopyable.h"
#include "Net/AddressFamily.h"
#include "Net/SocketType.h"
#include "Net/ProtocolType.h"
#include "Net/IPEndPoint.h"
#include "Threading/Tasks/Task.h"

class CORE_API Socket : public NonCopyable
{
	static constexpr size_t INVALID_SOCKET = -1;
	static constexpr int32 SOCKET_ERROR = -1;

	size_t _socket = INVALID_SOCKET;
	IPEndPoint _local;
	IPEndPoint _remote;

public:
	Socket(EAddressFamily af, ESocketType st, EProtocolType pt);
	~Socket() noexcept;

	void Close() noexcept;
	void Listen(int32 backlog = 0);
	void Bind(const EndPoint& ep);
	Task<> AcceptAsync(Socket& sock, std::stop_token cancellationToken = {});
	Task<> ConnectAsync(const EndPoint& ep, std::stop_token cancellationToken = {});
	Task<std::error_code> TryConnectAsync(const EndPoint& ep, std::stop_token cancellationToken = {});
	Task<size_t> SendAsync(std::span<const uint8> bytesToSend, std::stop_token cancellationToken = {});
	Task<> SendToAsync(const EndPoint& ep, std::span<const uint8> bytesToSend, std::stop_token cancellationToken = {});
	Task<size_t> ReceiveAsync(std::span<uint8> bytesToReceive, std::stop_token cancellationToken = {});
	Task<> ReceiveFromAsync(EndPoint& ep, std::span<uint8> bytesToRecv, std::stop_token cancellationToken = {});

	inline const IPEndPoint& GetLocalEndPoint() const { return _local; }
	inline const IPEndPoint& GetRemoteEndPoint() const { return _remote; }

public:
	template<class T>
	Task<size_t> SendAsync(std::span<const T> bytesToSend, std::stop_token cancellationToken = {})
	{
		return SendAsync(std::span<const uint8>((const uint8*)bytesToSend.data(), bytesToSend.size_bytes()), cancellationToken);
	}

	template<class T>
	Task<size_t> ReceiveAsync(std::span<T> bytesToReceive, std::stop_token cancellationToken = {})
	{
		return ReceiveAsync(std::span<uint8>((uint8*)bytesToReceive.data(), bytesToReceive.size_bytes()), cancellationToken);
	}

private:
	static void _trap_init();
	[[noreturn]] static void _throw_error();
};