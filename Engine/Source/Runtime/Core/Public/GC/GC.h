// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "StaticClass.h"

namespace Ayla
{
	struct CORE_API GC : public StaticClass
	{
		static void Collect();
	};
}