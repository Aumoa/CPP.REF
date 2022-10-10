// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "AddressFamily.h"
#include "SocketType.h"
#include "ProtocolType.h"
#include "EndPoint.h"
#include "Socket.gen.h"

SCLASS()
class NETWORK_API Socket : implements Object
{
	GENERATED_BODY()

public:
	struct Impl;

private:
	std::shared_ptr<Impl> _impl;

public:
	Socket();
	Socket(EAddressFamily addressFamily, ESocketType socketType, EProtocolType protocolType);
	~Socket() noexcept;

	void Bind(const EndPoint& ep);
	void Listen();
	void Listen(int32 backlog);
	std::unique_ptr<Socket> Accept();
	Task<size_t> ReceiveAsync(void* bufferToRecv, size_t len, std::stop_token cancellationToken = {});
	Task<size_t> SendAsync(const void* bufferToSend, size_t len, std::stop_token cancellationToken = {});

public:
	template<class T>
	Task<size_t> ReceiveAsync(std::span<T> bufferToRecv, std::stop_token cancellationToken = {})
	{
		return ReceiveAsync(bufferToRecv.data(), bufferToRecv.size_bytes(), cancellationToken);
	}

	template<class T>
	Task<size_t> ReceiveAsync(T& bufferToRecv, std::stop_token cancellationToken = {}) requires requires { { std::span(bufferToRecv) }; }
	{
		return ReceiveAsync(std::span(bufferToRecv), cancellationToken);
	}

	template<class T>
	Task<size_t> SendAsync(std::span<T> bufferToSend, std::stop_token cancellationToken = {})
	{
		return SendAsync(bufferToSend.data(), bufferToSend.size_bytes(), cancellationToken);
	}

	template<class T>
	Task<size_t> SendAsync(const T& bufferToSend, std::stop_token cancellationToken = {}) requires requires { { std::span(bufferToSend) }; }
	{
		return SendAsync(std::span(bufferToSend), cancellationToken);
	}
};