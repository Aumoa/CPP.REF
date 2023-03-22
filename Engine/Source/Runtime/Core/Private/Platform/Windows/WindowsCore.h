// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "CoreTypes/CharType.h"
#include "CoreTypes/IntegralTypes.h"
#include "CoreTypes/RealTypes.h"
#include "CoreTypes/String.h"

#pragma push_macro("TEXT")
#undef TEXT

#include <Windows.h>

#undef TEXT
#pragma pop_macro("TEXT")

#endif