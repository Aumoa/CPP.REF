// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "SocketType.h"
#include "Net/AddressFamily.h"
#include "Net/ProtocolType.h"
#include "Threading/Task.h"
#include <functional>

struct IPEndPoint;

class SOCKETS_API Socket
{
private:
	struct SocketImpl;
	std::shared_ptr<SocketImpl> Impl;

public:
	Socket() = default;
	Socket(const Socket&) = default;
	Socket(Socket&&) = default;

	Socket(EAddressFamily Family, ESocketType SocketType, EProtocolType ProtocolType);
	virtual ~Socket() noexcept;

	bool IsValid() const;
	Task<> Connect(const IPEndPoint& EndPoint);
	Task<> Send(const void* Buf, size_t Size);
	Task<size_t> Recv(void* OutBuf, size_t RecvSize, bool bVerifiedLength = false);
	void Bind(const IPEndPoint& endpoint);
	void Close();

	Socket& operator =(const Socket&) = default;
	Socket& operator =(Socket&&) = default;

	auto operator <=>(const Socket& InSock) const { return Impl <=> InSock.Impl; }
	bool operator ==(const Socket& InSock) const { return Impl == InSock.Impl; }

public:
	template<class TRandomAccess>
	Task<> Send(const TRandomAccess& Container) requires requires
	{
		{ std::size(Container) };
		{ std::data(Container) };
	}
	{
		using T = decltype(*std::data(Container));
		return Send((const char*)std::data(Container), sizeof(T) * std::size(Container));
	}

	template<class TRandomAccess>
	Task<size_t> Recv(TRandomAccess& Container, bool bVerifiedLength = false) requires requires
	{
		{ std::size(Container) } -> std::same_as<size_t>;
		{ std::data(Container) };
	}
	{
		using T = decltype(*std::data(Container));
		return Recv((char*)std::data(Container), std::size(Container), bVerifiedLength);
	}

public:
	static Socket NewTCPSocket();

public:
	bool IsReadyToRead(std::chrono::microseconds Timeout = std::chrono::microseconds(0));
	bool IsReadyToWrite(std::chrono::microseconds Timeout = std::chrono::microseconds(0));
};