// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"
#include "LanguageSupportMacros.h"

namespace Ayla
{
	enum class TaskCreationOptions : uint8
	{
		None = 0x00,
		LongRunning = 0x01
	};

	GENERATE_BITMASK_ENUM_OPERATORS(TaskCreationOptions)
}