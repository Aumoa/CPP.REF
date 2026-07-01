// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"
#include "Marshal/ObjectReferenceWrappers.h"
#include "Marshal/NativeExceptionInterop.h"

namespace Ayla
{
	struct CoreCLRFunctions
	{
		using signature__AsHardHandle__Invoke__t = NativeCallStatus(*)(ssize_t*);
		signature__AsHardHandle__Invoke__t m_AsHardHandle__Invoke;

		using signature__AsWeakHandle__Invoke__t = NativeCallStatus(*)(ssize_t*);
		signature__AsWeakHandle__Invoke__t m_AsWeakHandle__Invoke;

		using signature__CreateManagedInstancePtr__Invoke__t = NativeCallStatus(*)(ssize_t, ManagedObjectReferenceWrapper*);
		signature__CreateManagedInstancePtr__Invoke__t m_CreateManagedInstancePtr__Invoke;

		using signature__FreeGCHandlePtr__Invoke__t = NativeCallStatus(*)(ssize_t);
		signature__FreeGCHandlePtr__Invoke__t m_FreeGCHandlePtr__Invoke;

		using signature__ReleaseManagedExceptionPtr__Invoke__t = void(*)(uint64);
		signature__ReleaseManagedExceptionPtr__Invoke__t m_ReleaseManagedExceptionPtr__Invoke;
	};

	extern CoreCLRFunctions g_CoreCLRFunctions;

	void EnsureCoreCLRFunctionsInitialized();
}
