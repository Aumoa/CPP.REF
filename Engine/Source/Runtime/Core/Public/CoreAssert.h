// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/String.h"
#include "Exceptions/AssertException.h"
#include <string_view>
#include <source_location>

namespace libty::inline Core
{
	class CORE_API CoreAssert
	{
	public:
		static void Ensure(String exp, String msg, const std::source_location& location = std::source_location::current());
		static void DebugBreak();
	};
}

#if DO_CHECK

#define check(x) \
if (const bool b = (bool)(x); !b) \
{ \
	throw ::libty::Core::AssertException(TEXT(#x)); \
}

#define checkf(x, fmt, ...) \
if (const bool b = (bool)(x); !b) \
{ \
	throw ::libty::Core::AssertException(TEXT(#x), ::libty::Core::String::Format(fmt __VA_OPT__(,) __VA_ARGS__)); \
}

#define ensure(x) \
[&, b = (bool)(x)](const std::source_location& location = std::source_location::current())\
{\
	if (!b)\
	{\
		::libty::Core::CoreAssert::Ensure(TEXT(#x), TEXT(""), location);\
		static bool bSwitchLocal = true;\
		if (bSwitchLocal)\
		{\
			::libty::Core::CoreAssert::DebugBreak();\
			bSwitchLocal = false;\
		}\
	}\
	\
	return b;\
}()
#define ensureMsgf(x, fmt, ...) \
[&, b = (bool)(x)](const std::source_location& location = std::source_location::current())\
{\
	if (!b)\
	{\
		::libty::Core::CoreAssert::Ensure(TEXT(#x), ::libty::Core::String::Format(fmt __VA_OPT__(,) __VA_ARGS__), location);\
		static bool bSwitchLocal = true;\
		if (bSwitchLocal)\
		{\
			::libty::Core::CoreAssert::DebugBreak();\
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