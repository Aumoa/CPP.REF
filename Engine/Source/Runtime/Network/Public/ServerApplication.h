// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "NetworkApplication.h"
#include "ServerApplication.gen.h"

class ServiceSession;
class ClientSession;
class Socket;

SCLASS()
class NETWORK_API ServerApplication : public NetworkApplication
{
	GENERATED_BODY()

private:
	std::unique_ptr<ServiceSession> _server;
	std::map<int64, std::unique_ptr<ClientSession>> _sessions;
	std::vector<int64> _sessionIdPool;

	Spinlock _lock;
	std::vector<Socket> _pendingSockets;
	std::vector<Socket> _pendingSockets_buf;

public:
	ServerApplication();
	virtual ~ServerApplication() noexcept override;

	DECLARE_MULTICAST_EVENT(SessionEvent, ClientSession*);
	SessionEvent SessionConnected;
	SessionEvent SessionDisconnected;

protected:
	virtual void Start() override;
	virtual void Update(const TimeSpan& deltaTime);
	virtual void OnApplicationShutdown() noexcept override;

private:
	void OnSocketConnected(Socket sock);
	void OnSessionDisconnected(ClientSession* session);
};