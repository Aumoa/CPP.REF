// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Marshal/ObjectReferenceWrapper.h"
#include "Marshal/CoreCLRFunctions.h"
#include "Marshal/ManagedCallBoundary.h"
#include "Object.h"

namespace Ayla
{
	SharedPtr<Object> ObjectReferenceWrapper::AsNative_Internal() const
	{
		auto* ptr = reinterpret_cast<Object*>(Ptr);
		if (ptr)
		{
			if (IntGCHandlePtr)
			{
				EnsureCoreCLRFunctionsInitialized();
				ManagedCallBoundary::ThrowIfFailed(g_CoreCLRFunctions.m_FreeGCHandlePtr__Invoke(IntGCHandlePtr));
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
