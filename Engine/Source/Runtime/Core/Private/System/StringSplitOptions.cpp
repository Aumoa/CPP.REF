// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:StringSplitOptions;

export import :Std;

export enum class EStringSplitOptions
{
	None = 0,
	RemoveEmptyEntries = 0x1,
	TrimEntries = 0x2,
};

GENERATE_BITMASK_ENUM_OPERATORS(EStringSplitOptions, export);