// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SocketType.h"
#include "Net/AddressFamily.h"
#include "Net/ProtocolType.h"

struct IPEndPoint;

class SOCKETS_API SSocket : implements SObject
{
	GENERATED_BODY(SSocket)

private:
	struct SocketImpl;
	std::unique_ptr<SocketImpl> Impl;
	bool bClosed = false;

public:
	SSocket(EAddressFamily Family, ESocketType SocketType, EProtocolType ProtocolType);
	virtual ~SSocket() noexcept override;

	Task<> Connect(const IPEndPoint& EndPoint);
	Task<> Bind(const IPEndPoint& EndPoint);
	void Close();

public:
	static SSocket* NewTCPSocket();

public:
	bool IsReadyToRead(std::chrono::microseconds Timeout = std::chrono::microseconds(0)) const;
	bool IsReadyToWrite(std::chrono::microseconds Timeout = std::chrono::microseconds(0)) const;
};