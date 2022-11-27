// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Services/IHostedService.h"
#include "Json/JsonUtility.h"

class IServiceProvider;

class ServiceSession : public IHostedService
{
public:
	struct Configuration
	{
		int32 ServicePort;
		GENERATE_JSON_BODY(Configuration, ServicePort);
	};

private:
	Configuration _config;
	std::unique_ptr<Socket> _sock;
	std::stop_source _ss;
	Task<> _acceptor;

public:
	ServiceSession(IServiceProvider* sp);
	virtual ~ServiceSession() noexcept override;

	virtual Task<> StartAsync(std::stop_token cancellationToken);
	virtual Task<> StopAsync(std::stop_token cancellationToken);

private:
	Task<> StartSocketAcceptor(std::stop_token cancellationToken, TaskCompletionSource<> init);
};