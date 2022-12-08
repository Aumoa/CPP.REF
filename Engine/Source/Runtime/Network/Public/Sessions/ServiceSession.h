// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Services/IHostedService.h"
#include "Json/JsonUtility.h"

class IServiceProvider;
class ClientSession;

class ServiceSession : public IHostedService
{
private:
	std::unique_ptr<Socket> _sock;
	std::stop_source _ss;
	Task<> _acceptor;

public:
	ServiceSession(IServiceProvider* sp);
	virtual ~ServiceSession() noexcept override;

	virtual Task<> StartAsync(std::stop_token cancellationToken);
	virtual Task<> StopAsync(std::stop_token cancellationToken);

	Task<> StartService(int32 servicePort);

protected:
	virtual void OnSessionConnected(std::shared_ptr<ClientSession> session) = 0;

private:
	Task<> StartSocketAcceptor(std::stop_token cancellationToken, TaskCompletionSource<> init);
};