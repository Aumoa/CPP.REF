// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string_view>
#include <format>
#include "PrimitiveTypes.h"

class CORESOBJECT_API CoreAssert
{
public:
	static void Assert(std::wstring_view msg, std::wstring_view file, int32 line);
	static void Ensure(std::wstring_view msg, std::wstring_view file, int32 line);
	static void DebugBreak();
};

#if DO_CHECK

#define check(x) if (const bool b = (bool)(x); !b) { CoreAssert::Assert(L ## #x, __FILEW__, __LINE__); }
#define checkf(x, fmt, ...) if (const bool b = (bool)(x); !b) { CoreAssert::Assert(std::format(fmt __VA_OPT__(,) __VA_ARGS__), __FILEW__, __LINE__); }
#define ensure(x) \
[b = (bool)(x)]()\
{\
	if (!b)\
	{\
		CoreAssert::Ensure(L ## #x, __FILEW__, __LINE__);\
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
[b = (bool)(x)]()\
{\
	if (!b)\
	{\
		CoreAssert::Ensure(std::format(fmt __VA_OPT__(,) __VA_ARGS__), __FILEW__, __LINE__);\
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
#define ensure(x) (x)
#define ensureMsgf(x, format, ...) (x)

#endif