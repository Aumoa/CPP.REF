// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class IHostedService
{
public:
	virtual ~IHostedService() noexcept = default;

	virtual Task<> StartAsync(std::stop_token cancellationToken) = 0;
	virtual Task<> StopAsync(std::stop_token cancellationToken) = 0;
};