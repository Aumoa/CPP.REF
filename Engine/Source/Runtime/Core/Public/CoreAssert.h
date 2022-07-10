// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/String.h"
#include "Misc/noop.h"
#include "Exceptions/AssertException.h"
#include <source_location>

class CORE_API CoreAssert
{
public:
	static void Assert(const String& exp, const String& msg);
	static void DebugBreak();
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
			CoreAssert::DebugBreak();\
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
			CoreAssert::DebugBreak();\
			bSwitchLocal = false;\
		}\
	}\
	\
	return b;\
}()

#else

#define check(x)
#define checkf(x, format, ...)
#define ensure(x) noop(x)
#define ensureMsgf(x, format, ...) noop(x)

#endif