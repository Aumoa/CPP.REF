// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"

namespace Ayla
{
	struct CoreCLRFunctions
	{
		using signature__AsHardHandle__Invoke__t = void(*)(ssize_t*);
		signature__AsHardHandle__Invoke__t AsHardHandle__Invoke;

		using signature__AsWeakHandle__Invoke__t = void(*)(ssize_t*);
		signature__AsWeakHandle__Invoke__t AsWeakHandle__Invoke;
	};
}