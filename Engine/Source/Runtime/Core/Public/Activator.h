// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "StaticClass.h"
#include "Object.h"
#include "SharedPtr.h"

namespace Ayla
{
	class CORE_API Activator : public StaticClass
	{
	public:
		static SharedPtr<Object> CreateInstance(ManagedTypeWrapper type);
	};
}