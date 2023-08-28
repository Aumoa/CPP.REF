// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

// Whether the CPU is x86/x64 (i.e. both 32 and 64-bit variants)
#ifndef PLATFORM_CPU_X86_FAMILY
#	if (defined(_M_IX86) || defined(__i386__) || defined(_M_X64) || defined(__amd64__) || defined(__x86_64__))
#		define PLATFORM_CPU_X86_FAMILY	1
#	else
#		define PLATFORM_CPU_X86_FAMILY	0
#	endif
#endif

// Whether the CPU is AArch32/AArch64 (i.e. both 32 and 64-bit variants)
#ifndef PLATFORM_CPU_ARM_FAMILY
#	if (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__) || defined(_M_ARM64))
#		define PLATFORM_CPU_ARM_FAMILY	1
#	else
#		define PLATFORM_CPU_ARM_FAMILY	0
#	endif
#endif


// intrinsic functions.
#if PLATFORM_CPU_X86_FAMILY
#include <emmintrin.h>
#endif

#if !defined(__clang__) && !defined(__GNUC__)
#	include <intrin.h>
#	if defined(_M_ARM)
#		include <armintr.h>
#	elif defined(_M_ARM64)
#		include <arm64intr.h>
#	endif
#endif


#if PLATFORM_CPU_X86_FAMILY
#	define PLATFORM_YIELD() _mm_pause()
#elif PLATFORM_CPU_ARM_FAMILY
#	if !defined(__clang__)
#		define PLATFORM_YIELD() __yield()
#	else
#		define PLATFORM_YIELD() __builtin_arm_yield()
#	endif
#else
#	error Unsupported architecture!
#endif


#if !defined(PLATFORM_WINDOWS)
#	define PLATFORM_WINDOWS 0
#endif

#if !defined(PLATFORM_LINUX)
#	define PLATFORM_LINUX 0
#endif

#if !defined(PLATFORM_UNIX)
#	define PLATFORM_UNIX 0
#endif

#if !defined(PLATFORM_COMPILER_CLANG)
#	define PLATFORM_COMPILER_CLANG 0
#endif

#if (defined(__clang__) || defined(__GNU__)) && PLATFORM_UNIX
#	define LIKELY(X) __builtin_expect(!!(X), 1)
#	define UNLIKELY(X) __builtin_expect(!!(X), 0)
#else
#	define LIKELY(X) (X)
#	define UNLIKELY(X) (X)
#endif