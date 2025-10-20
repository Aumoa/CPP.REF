// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"
#include "Marshal/ObjectReferenceWrapper.h"

namespace Ayla
{
	struct CoreCLRFunctions
	{
		using signature__AsHardHandle__Invoke__t = void(*)(ssize_t*);
		signature__AsHardHandle__Invoke__t AsHardHandle__Invoke;

		using signature__AsWeakHandle__Invoke__t = void(*)(ssize_t*);
		signature__AsWeakHandle__Invoke__t AsWeakHandle__Invoke;

		using signature__CreateManagedInstancePtr__Invoke__t = ObjectReferenceWrapper(*)(ssize_t);
		signature__CreateManagedInstancePtr__Invoke__t CreateManagedInstancePtr__Invoke;

		using signature__FreeGCHandlePtr__Invoke__t = void(*)(ssize_t);
		signature__FreeGCHandlePtr__Invoke__t FreeGCHandlePtr__Invoke;
	};
}