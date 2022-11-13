// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/EnumFlags.h"

enum class EFileSharedMode
{
	None = 0,
	Read = 0x1,
	Write = 0x2,
	Delete = 0x4
};

DEFINE_ENUM_FLAGS_OPERATORS(EFileSharedMode);