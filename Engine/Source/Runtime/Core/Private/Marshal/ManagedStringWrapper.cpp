// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Marshal/ManagedStringWrapper.h"

extern "C"
{
	PLATFORM_SHARED_EXPORT void Ayla__ManagedStringWrapper__FreeIntRef__Injected(::Ayla::ssize_t intRef)
	{
		delete reinterpret_cast<::Ayla::String*>(intRef);
	}
}