// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Marshal/ObjectReferenceWrapper.h"
#include "Marshal/CoreCLRFunctions.h"
#include "Object.h"

namespace Ayla
{
	extern CoreCLRFunctions g_CoreCLRFunctions;

	SharedPtr<Object> ObjectReferenceWrapper::AsNative_Internal() const
	{
		auto* ptr = reinterpret_cast<Object*>(Ptr);
		if (ptr)
		{
			if (IntGCHandlePtr)
			{
				g_CoreCLRFunctions.FreeGCHandlePtr__Invoke(IntGCHandlePtr);
			}

			return ptr->AsShared();
		}
		else
		{
			return nullptr;
		}
	}

	ObjectReferenceWrapper ObjectReferenceWrapper::FromObject_Internal(Object* obj)
	{
		obj->AddRef();
		return obj->AsWrapper();
	}
}