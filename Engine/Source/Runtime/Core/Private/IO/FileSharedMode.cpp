// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:FileSharedMode;

export import :IntegralTypes;

export enum class EFileSharedMode : uint32
{
	None = 0,
	Read = 0x1,
	Write = 0x2,
	Delete = 0x4
};

GENERATE_BITMASK_ENUM_OPERATORS(EFileSharedMode, export);