// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Marshal/ManagedStringWrapper.h"

extern "C"
{
	PLATFORM_SHARED_EXPORT void Ayla__ManagedStringWrapper__FreeIntRef__Injected(::Ayla::ssize_t intRef)
	{
		if constexpr (sizeof(wchar_t) == 2)
		{
			delete reinterpret_cast<::Ayla::String*>(intRef);
		}
		else
		{
			delete reinterpret_cast<char16_t*>(intRef);
		}
	}
}