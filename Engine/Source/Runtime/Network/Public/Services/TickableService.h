// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Services/IHostedService.h"
#include "Json/JsonUtility.h"

class IServiceProvider;
class IConfiguration;

class NETWORK_API TickableService : public IHostedService
{
public:
	struct Configuration
	{
		double TickInterval = 0;
		GENERATE_JSON_BODY(Configuration, TickInterval);
	};

private:
	Configuration _config;
	TaskCompletionSource<> _tcs;
	std::jthread _wt;
	Spinlock _mtx;
	SpinlockConditionVariable _cv;

public:
	TickableService(IServiceProvider* provider);

	virtual Task<> StartAsync(std::stop_token cancellationToken) override;
	virtual Task<> StopAsync(std::stop_token cancellationToken) override;

protected:
	virtual void Tick(const TimeSpan& deltaTime) = 0;

private:
	void _worker(std::stop_token cancellationToken);
};