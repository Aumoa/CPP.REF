// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "Platform/Generic/GenericPlatformMacros.h"

#define FORCEINLINE __forceinline
#define FORCEINLINE_LAMBDA [[msvc::forceinline]]
#define FORCENOINLINE __declspec(noinline)
#define PLATFORM_BREAK() (__nop(), __debugbreak())
#define PLATFORM_UNREFERENCED_PARAMETER(X) __noop(X)

#endif