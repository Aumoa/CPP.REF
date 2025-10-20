// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Activator.h"
#include "Marshal/CoreCLRFunctions.h"

namespace Ayla
{
	extern CoreCLRFunctions g_CoreCLRFunctions;

	SharedPtr<Object> Activator::CreateInstance(ManagedTypeWrapper type)
	{
		auto wrapper = g_CoreCLRFunctions.CreateManagedInstancePtr__Invoke((ssize_t)type.ScriptTypeGetter);
		return wrapper.AsNative<Object>();
	}
}