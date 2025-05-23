// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "Platform/Generic/GenericPlatformMacros.h"

#define FORCEINLINE __forceinline
#define FORCEINLINE_LAMBDA [[msvc::forceinline]]
#define FORCENOINLINE __declspec(noinline)
#define PLATFORM_BREAK() (__nop(), __debugbreak())
#define PLATFORM_UNREFERENCED_PARAMETER(X) __noop(X)
#define PLATFORM_SHARED_EXPORT __declspec(dllexport)
#define PLATFORM_SHARED_IMPORT __declspec(dllimport)
#define PLATFORM_NEWLINE TEXT("\r\n")

#endif