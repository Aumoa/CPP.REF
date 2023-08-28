// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_UNIX

#include "Platform/Generic/GenericPlatformMacros.h"

#define FORCEINLINE inline __attribute__ ((always_inline))
#define FORCEINLINE_LAMBDA __attribute__ ((always_inline))
#define FORCENOINLINE __attribute__ ((noinline))

#if PLATFORM_CPU_X86_FAMILY
#	define PLATFORM_BREAK() __asm__ volatile("int $0x03")
#else
#	define PLATFORM_BREAK() raise(SIGTRAP)
#endif

#define PLATFORM_UNREFERENCED_PARAMETER(X) ((void)(X))

#endif