// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Reflection/ObjectReferenceWrapper.h"
#include "Object.h"

namespace Ayla
{
	std::shared_ptr<Object> ObjectReferenceWrapper::AsNative_Internal() const
	{
		auto* ptr = reinterpret_cast<Object*>(Ptr);
		if (ptr)
		{
			return ptr->shared_from_this();
		}
		else
		{
			return nullptr;
		}
	}
}