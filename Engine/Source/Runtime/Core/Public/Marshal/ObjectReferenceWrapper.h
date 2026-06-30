// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"
#include "SharedPtr.h"
#include <memory>

namespace Ayla
{
	class Object;

	struct CORE_API NativeObjectReferenceWrapper
	{
		ssize_t Ptr;
		ssize_t IntGCHandlePtr;
		uint64 GCHandleSerial;

		template<class T>
		static NativeObjectReferenceWrapper FromObject(const SharedPtr<T>& obj)
		{
			return obj ? FromObject_Internal(obj.Get()) : NativeObjectReferenceWrapper{};
		}

	private:
		static NativeObjectReferenceWrapper FromObject_Internal(Object* obj);
	};

	struct CORE_API ManagedObjectReferenceWrapper
	{
		ssize_t Ptr;
		ssize_t IntGCHandlePtr;
		uint64 GCHandleSerial;

		template<class T>
		inline SharedPtr<T> AsNative()
		{
			return AsNative_Internal().As<T>();
		}

	private:
		SharedPtr<Object> AsNative_Internal();
	};

	struct CORE_API BoundObjectReferenceWrapper
	{
		ssize_t Ptr;
		uint64 GCHandleSerial;
	};

	struct CORE_API ObjectReferenceWrapper
	{
		ssize_t Ptr;
		ssize_t IntGCHandlePtr;
		uint64 GCHandleSerial;

		template<class T>
		inline SharedPtr<T> AsNative()
		{
			return AsNative_Internal().As<T>();
		}

		template<class T>
		static ObjectReferenceWrapper FromObject(const SharedPtr<T>& obj)
		{
			return obj ? FromObject_Internal(obj.Get()) : ObjectReferenceWrapper{};
		}

	private:
		SharedPtr<Object> AsNative_Internal();

		static ObjectReferenceWrapper FromObject_Internal(Object* obj);
	};
}
