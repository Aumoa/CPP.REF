// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Socket.h"

class NETWORK_API SecuredSocket : public Socket
{
public:
	//virtual std::unique_ptr<Socket> Accept() override;
	//virtual Task<size_t> ReceiveAsync(void* bufferToRecv, size_t len, std::stop_token cancellationToken = {}) override;
	//virtual Task<size_t> SendAsync(const void* bufferToSend, size_t len, std::stop_token cancellationToken = {}) override;
};