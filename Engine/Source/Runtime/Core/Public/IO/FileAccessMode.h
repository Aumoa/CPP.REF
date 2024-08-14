// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "System/IntegralTypes.h"
#include "System/LanguageSupportMacros.h"

enum class EFileAccessMode : uint32
{
	Read = 0x80000000L,
	Write = 0x40000000L,
	Append = 0x00000004L,
	All = 0x10000000L
};

GENERATE_BITMASK_ENUM_OPERATORS(EFileAccessMode);