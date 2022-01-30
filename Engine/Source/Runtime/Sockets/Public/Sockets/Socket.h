// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SocketType.h"
#include "Net/AddressFamily.h"
#include "Net/ProtocolType.h"
#include <functional>

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
	Task<> Send(const void* Buf, size_t Size);
	Task<size_t> Receive(void* OutBuf, size_t RecvSize, bool bVerifiedLength = false);
	void Close();

public:
	Task<> Send(std::string_view StringBuf) { return Send(StringBuf.data(), StringBuf.size()); }
	Task<> Send(std::wstring_view StringBuf) { return Send(StringBuf.data(), StringBuf.size() * sizeof(wchar_t)); }

public:
	static SSocket* NewTCPSocket();

public:
	bool IsReadyToRead(std::chrono::microseconds Timeout = std::chrono::microseconds(0));
	bool IsReadyToWrite(std::chrono::microseconds Timeout = std::chrono::microseconds(0));
};