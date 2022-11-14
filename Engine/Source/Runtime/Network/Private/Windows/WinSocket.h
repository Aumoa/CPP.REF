// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ImplSocket.h"

#if PLATFORM_WINDOWS

#include "WindowsMinimal.h"
#include "Net/AddressFamily.h"
#include "Net/SocketType.h"
#include "Net/ProtocolType.h"
#include "WinOverlappedContainer.h"

//class WinSocket : public ImplSocket
//{
//private:
//	SOCKET _socket;
//	WinOverlappedContainer* _overlaps = nullptr;
//
//	LPFN_CONNECTEX WSAConnectEx = nullptr;
//	LPFN_ACCEPTEX WSAAcceptEx = nullptr;
//	LPFN_GETACCEPTEXSOCKADDRS WSAGetAcceptExSockaddrs = nullptr;
//
//public:
//	WinSocket();
//	virtual ~WinSocket() noexcept override;
//
//	virtual const void* GetSocket() const noexcept override;
//
//	virtual void Bind(const IPEndPoint& ep) override;
//	virtual Task<> ConnectAsync(const IPEndPoint& ep) override;
//	virtual void Listen() override;
//	virtual Task<std::shared_ptr<ImplSocket>> AcceptAsync() override;
//	virtual Task<size_t> ReceiveAsync(void* bufferToRecv, size_t len, std::stop_token cancellationToken = {}) override;
//	virtual Task<size_t> SendAsync(const void* bufferToSend, size_t len, std::stop_token cancellationToken = {}) override;
//	virtual void Close() override;
//};

#endif