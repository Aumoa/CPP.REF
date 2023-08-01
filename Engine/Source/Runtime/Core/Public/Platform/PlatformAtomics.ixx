// Copyright 2020-2022 Aumoa.lib. All right reserved.

module;

#if PLATFORM_WINDOWS
#include "Platform/Windows/WindowsCore.h"
#undef InterlockedIncrement
#endif

export module Core.Platform:PlatformAtomics;

import Core.Std;
import Core.System;

export class PlatformAtomics : StaticClass
{
public:
	static FORCEINLINE int8 InterlockedIncrement(volatile int8* Value);
	static FORCEINLINE int16 InterlockedIncrement(volatile int16* Value);
	static FORCEINLINE int32 InterlockedIncrement(volatile int32* Value);
	static FORCEINLINE int64 InterlockedIncrement(volatile int64* Value);
};

#if PLATFORM_WINDOWS
#include "Platform/Windows/WindowsPlatformAtomics.h"
#endif