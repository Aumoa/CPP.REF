// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"

namespace Ayla
{
	struct ManagedCancellationTokenWrapper
	{
		ssize_t Handle;
		ssize_t Ptr;
	};
}