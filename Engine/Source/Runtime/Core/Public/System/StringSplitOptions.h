// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <type_traits>
#include "System/LanguageSupportMacros.h"

enum class EStringSplitOptions
{
	None = 0,
	RemoveEmptyEntries = 0x1,
	TrimEntries = 0x2,
};

GENERATE_BITMASK_ENUM_OPERATORS(EStringSplitOptions);