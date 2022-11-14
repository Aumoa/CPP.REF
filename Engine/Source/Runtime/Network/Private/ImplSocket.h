// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/IPEndPoint.h"

class Socket;

//class ImplSocket
//{
//public:
//	ImplSocket();
//	virtual ~ImplSocket() noexcept;
//
//	virtual const void* GetSocket() const noexcept = 0;
//
//	virtual void Bind(const IPEndPoint& ep) = 0;
//	virtual Task<> ConnectAsync(const IPEndPoint& ep) = 0;
//	virtual void Listen() = 0;
//	virtual Task<std::shared_ptr<ImplSocket>> AcceptAsync() = 0;
//	virtual Task<size_t> ReceiveAsync(void* bufferToRecv, size_t len, std::stop_token cancellationToken = {}) = 0;
//	virtual Task<size_t> SendAsync(const void* bufferToSend, size_t len, std::stop_token cancellationToken = {}) = 0;
//	virtual void Close() = 0;
//};