// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"

namespace libty::inline Core
{
	struct CORESOBJECT_API IHostedService : implements(SObject)
	{
		GENERATED_INTERFACE_BODY(IHostedService);

	public:
		virtual Task<> StartAsync(std::stop_token cancellationToken = {}) = 0;
		virtual Task<> StopAsync(std::stop_token cancellationToken = {}) = 0;
	};
}