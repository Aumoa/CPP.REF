// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_LINUX

#include "Platform/Generic/GenericPlatformMacros.h"
#include <signal.h>

#define FORCEINLINE __attribute__((always_inline))
#define FORCEINLINE_LAMBDA __attribute__((always_inline)) inline
#define FORCENOINLINE __attribute__((noinline))
#define PLATFORM_BREAK() (::raise(SIGTRAP))
#define PLATFORM_UNREFERENCED_PARAMETER(X) (void)sizeof((int[]){(X, 0)})
#define PLATFORM_SHARED_EXPORT __attribute__((visibility("default")))
#define PLATFORM_SHARED_IMPORT 
#define PLATFORM_NEWLINE TEXT("\n")

#endif
