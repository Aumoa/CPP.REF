// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Threading/Thread.h"
#include "ServiceSession.gen.h"

class Socket;

SCLASS()
class NETWORK_API ServiceSession : implements Object
{
	GENERATED_BODY()

private:
	std::unique_ptr<Socket> _sock;
	Thread _acceptor;
	std::stop_source _ss;

public:
	ServiceSession();
	virtual ~ServiceSession() noexcept override;

	void Start();
	void CloseSession() noexcept;

	DECLARE_MULTICAST_EVENT(SocketConnectedEvent, Socket*);
	SocketConnectedEvent SocketConnected;

private:
	void StartSocketAcceptor(std::stop_token cancellationToken);
};