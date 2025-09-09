// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"
#include "GC/RPtr.h"

namespace Ayla
{
	class Object;

	struct ObjectReferenceWrapper
	{
		ssize_t InstanceId;
		ssize_t Handle;
		int32 Flags;

		template<class T>
		inline RPtr<T> Resolve() const
		{
			return RPtr<T>(dynamic_cast<T*>(reinterpret_cast<Object*>(InstanceId)));
		}
	};
}