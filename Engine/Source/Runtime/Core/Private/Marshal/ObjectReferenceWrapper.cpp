// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Marshal/ObjectReferenceWrapper.h"
#include "Marshal/CoreCLRFunctions.h"
#include "Marshal/ManagedCallBoundary.h"
#include "Object.h"

namespace Ayla
{
	SharedPtr<Object> ManagedObjectReferenceWrapper::AsNative_Internal()
	{
		auto* ptr = reinterpret_cast<Object*>(Ptr);
		if (ptr)
		{
			auto intGCHandlePtr = IntGCHandlePtr;
			IntGCHandlePtr = 0;
			if (intGCHandlePtr)
			{
				EnsureCoreCLRFunctionsInitialized();
				ManagedCallBoundary::ThrowIfFailed(g_CoreCLRFunctions.m_FreeGCHandlePtr__Invoke(intGCHandlePtr));
			}

			return ptr->AsShared();
		}
		else
		{
			return nullptr;
		}
	}

	NativeObjectReferenceWrapper NativeObjectReferenceWrapper::FromObject_Internal(Object* obj)
	{
		obj->AddRef();
		auto wrapper = obj->AsWrapper();
		return NativeObjectReferenceWrapper
		{
			.Ptr = wrapper.Ptr,
			.IntGCHandlePtr = wrapper.IntGCHandlePtr,
			.GCHandleSerial = wrapper.GCHandleSerial
		};
	}

	SharedPtr<Object> ObjectReferenceWrapper::AsNative_Internal()
	{
		ManagedObjectReferenceWrapper wrapper
		{
			.Ptr = Ptr,
			.IntGCHandlePtr = IntGCHandlePtr,
			.GCHandleSerial = GCHandleSerial
		};

		auto result = wrapper.AsNative<Object>();
		IntGCHandlePtr = wrapper.IntGCHandlePtr;
		return result;
	}

	ObjectReferenceWrapper ObjectReferenceWrapper::FromObject_Internal(Object* obj)
	{
		obj->AddRef();
		return obj->AsWrapper();
	}
}
