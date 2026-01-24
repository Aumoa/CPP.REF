// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "IntegralTypes.h"
#include "StaticClass.h"
#include <span>

namespace Ayla
{
	class CORE_API Encoding : public StaticClass
	{
	public:
		static size_t TryParseBOM(std::span<const char> c_str, int32* outCodepage);
	};
}