// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/String.h"
#include "Misc/PlatformMisc.h"
#include <source_location>

class CORE_API CoreAssert
{
public:
	[[noreturn]]
	static void Assert(const String& exp, const String& msg);
};

#if DO_CHECK

#define check(x) \
if (const bool b = (bool)(x); !b) \
{ \
	CoreAssert::Assert(TEXT(#x), TEXT("Assertion failure.")); \
}

#define checkf(x, fmt, ...) \
if (const bool b = (bool)(x); !b) \
{ \
	CoreAssert::Assert(TEXT(#x), String::Format(fmt __VA_OPT__(, __VA_ARGS__))); \
}

#define ensure(x) \
[&, b = (bool)(x)]()\
{\
	if (!b)\
	{\
		CoreAssert::Assert(TEXT(#x), TEXT("Ensure failure."));\
		static bool bSwitchLocal = true;\
		if (bSwitchLocal)\
		{\
			PlatformMisc::Debugbreak();\
			bSwitchLocal = false;\
		}\
	}\
	\
	return b;\
}()
#define ensureMsgf(x, fmt, ...) \
[&, b = (bool)(x)]()\
{\
	if (!b)\
	{\
		CoreAssert::Assert(TEXT(#x), String::Format(fmt __VA_OPT__(,) __VA_ARGS__));\
		static bool bSwitchLocal = true;\
		if (bSwitchLocal)\
		{\
			PlatformMisc::Debugbreak();\
			bSwitchLocal = false;\
		}\
	}\
	\
	return b;\
}()

#else

#define check(x)
#define checkf(x, format, ...)
#define ensure(x) (x)
#define ensureMsgf(x, format, ...) (x)

#endif