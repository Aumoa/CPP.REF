// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:FileAccessMode;

export import :IntegralTypes;

export enum class EFileAccessMode : uint32
{
	Read = 0x80000000L,
	Write = 0x40000000L,
	Append = 0x00000004L,
	All = 0x10000000L
};

GENERATE_BITMASK_ENUM_OPERATORS(EFileAccessMode, export);