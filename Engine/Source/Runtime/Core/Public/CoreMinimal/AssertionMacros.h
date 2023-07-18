// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if DO_CHECK
#define check(X) \
if (const bool b = (bool)(x); !b) \
{ \
    CoreAssert::Assert(TEXT(#x), TEXT("Assertion failure.")); \
}

#define checkf(X, Fmt, ...) \
if (const bool b = (bool)(x); !b) \
{ \
    CoreAssert::Assert(TEXT(#x), String::Format(Fmt __VA_OPT__(, __VA_ARGS__))); \
}

#define ensure(X) \
[&, b = (bool)(x)]() \
{ \
    if (!b) \
    { \
        CoreAssert::Assert(TEXT(#x), TEXT("Ensure failure.")); \
        static bool bSwitchLocal = true; \
        if (bSwitchLocal) \
        { \
            PlatformMisc::Debugbreak(); \
            bSwitchLocal = false; \
        } \
    } \
    \
    return b; \
}

#define ensureMsgf(X, Fmt, ...) \
[&, b = (bool)(x)]() \
{ \
    if (!b) \
    { \
        CoreAssert::Assert(TEXT(#x), String::Format(Fmt __VA_OPT__(, __VA_ARGS__))); \
        static bool bSwitchLocal = true; \
        if (bSwitchLocal) \
        { \
            PlatformMisc::Debugbreak(); \
            bSwitchLocal = false; \
        } \
    } \
    \
    return b; \
}
#else
#define check(X)
#define ensure(X) ((bool)(X))
#define checkf(X, Format, ...)
#define ensureMsgf(X, Format, ...) ((bool)(X))
#endif