// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <csignal>

#define FORCEINLINE __attribute__((always_inline)) inline
#define FORCEINLINE_LAMBDA 
#define FORCENOINLINE __attribute__((noinline))
#define PLATFORM_BREAK() (::raise(SIGTRAP))
#define PLATFORM_UNREFERENCED_PARAMETER(X) (void)(X)
#define PLATFORM_SHARED_EXPORT __attribute__((visibility("default")))
#define PLATFORM_SHARED_IMPORT 
#define PLATFORM_NEWLINE TEXT("\n")
#define PLATFORM_NOVTABLE
