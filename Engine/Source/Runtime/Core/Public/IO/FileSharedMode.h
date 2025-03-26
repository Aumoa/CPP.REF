// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "LanguageSupportMacros.h"

namespace Ayla
{
	enum class FileSharedMode
	{
		None = 0,
		Read = 0x1,
		Write = 0x2,
		Delete = 0x4
	};
}

GENERATE_BITMASK_ENUM_OPERATORS(::Ayla::FileSharedMode);