// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Reflection/ObjectReferenceWrapper.h"
#include "Object.h"

namespace Ayla
{
	std::shared_ptr<Object> ObjectReferenceWrapper::Resolve_Internal() const
	{
		return reinterpret_cast<Object*>(Ptr)->shared_from_this();
	}
}