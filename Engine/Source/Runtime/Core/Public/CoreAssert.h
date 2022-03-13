// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <string_view>
#include <format>
#include <source_location>

class CORE_API CoreAssert
{
public:
	static void Assert(std::wstring_view msg, const std::source_location& location = std::source_location::current());
	static void Ensure(std::wstring_view msg, const std::source_location& location = std::source_location::current());
	static void DebugBreak();
};

#if DO_CHECK

#define checkf(x, fmt, ...) if (const bool b = (bool)(x); !b) { CoreAssert::Assert(std::format(fmt __VA_OPT__(,) __VA_ARGS__)); }
#define ensure(x) \
[b = (bool)(x)](const std::source_location& location = std::source_location::current())\
{\
	if (!b)\
	{\
		CoreAssert::Ensure(L ## #x, location);\
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
[b = (bool)(x)](const std::source_location& location = std::source_location::current())\
{\
	if (!b)\
	{\
		CoreAssert::Ensure(std::format(fmt __VA_OPT__(,) __VA_ARGS__), location);\
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

#define checkf(x, format, ...)
#define ensure(x) (x)
#define ensureMsgf(x, format, ...) (x)

#endif