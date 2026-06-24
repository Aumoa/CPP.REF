// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Activator.h"
#include "Marshal/CoreCLRFunctions.h"
#include "Marshal/ManagedCallBoundary.h"

namespace Ayla
{
	extern CoreCLRFunctions g_CoreCLRFunctions;

	SharedPtr<Object> Activator::CreateInstance(ManagedTypeWrapper type)
	{
		ObjectReferenceWrapper wrapper{};
		ManagedCallBoundary::ThrowIfFailed(g_CoreCLRFunctions.m_CreateManagedInstancePtr__Invoke((ssize_t)type.ScriptTypeGetter, &wrapper));
		return wrapper.AsNative<Object>();
	}
}
