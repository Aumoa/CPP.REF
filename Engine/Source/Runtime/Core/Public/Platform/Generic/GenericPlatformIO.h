// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "StaticClass.h"
#include "Platform/PlatformMacros.h"

namespace Ayla
{
	struct CORE_API GenericPlatformIO : public StaticClass
	{
		static constexpr size_t OVERLAPPED_SIZE = 1;
	};
}