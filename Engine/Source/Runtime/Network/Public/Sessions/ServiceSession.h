// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Services/IHostedService.h"
#include "Json/JsonUtility.h"

class IServiceProvider;

class ServiceSession : IHostedService
{
public:
	struct Configuration
	{
		int32 ServicePort;
		GENERATE_JSON_BODY(Configuration, ServicePort);
	};

public:
	ServiceSession(IServiceProvider* sp);

	virtual Task<> StartAsync(std::stop_token cancellationToken);
	virtual Task<> StopAsync(std::stop_token cancellationToken);
};