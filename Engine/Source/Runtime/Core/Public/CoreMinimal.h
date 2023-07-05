// Copyright 2020-2022 Aumoa.lib. All right reserved.

// for String.h
#define TEXT(X) String::FromLiteral(L ## X)

// for CoreAssert.h
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
#define ensure(X)
#define checkf(X, Format, ...)
#define ensureMsgf(X, Format, ...)
#endif

// for nameof.h
#define nameof(x) lib::details::__nameof_helper<decltype(x)>(TEXT(#x))

// for RecursiveMacroHelper.h
#define MACRO_RECURSIVE_HELPER_PARENTHESES ()

#define MACRO_RECURSIVE_HELPER_EXPAND_0(...) \
 MACRO_RECURSIVE_HELPER_EXPAND_1( \
  MACRO_RECURSIVE_HELPER_EXPAND_1( \
   MACRO_RECURSIVE_HELPER_EXPAND_1( \
    MACRO_RECURSIVE_HELPER_EXPAND_1(__VA_ARGS__) \
   ) \
  ) \
 )

#define MACRO_RECURSIVE_HELPER_EXPAND_1(...) \
 MACRO_RECURSIVE_HELPER_EXPAND_2( \
  MACRO_RECURSIVE_HELPER_EXPAND_2( \
   MACRO_RECURSIVE_HELPER_EXPAND_2( \
    MACRO_RECURSIVE_HELPER_EXPAND_2(__VA_ARGS__) \
   ) \
  ) \
 )

#define MACRO_RECURSIVE_HELPER_EXPAND_2(...) \
 MACRO_RECURSIVE_HELPER_EXPAND_3( \
  MACRO_RECURSIVE_HELPER_EXPAND_3( \
   MACRO_RECURSIVE_HELPER_EXPAND_3( \
    MACRO_RECURSIVE_HELPER_EXPAND_3(__VA_ARGS__) \
   ) \
  ) \
 )

#define MACRO_RECURSIVE_HELPER_EXPAND_3(...) \
 MACRO_RECURSIVE_HELPER_EXPAND_4( \
  MACRO_RECURSIVE_HELPER_EXPAND_4( \
   MACRO_RECURSIVE_HELPER_EXPAND_4( \
    MACRO_RECURSIVE_HELPER_EXPAND_4(__VA_ARGS__) \
   ) \
  ) \
 )

#define MACRO_RECURSIVE_HELPER_EXPAND_4(...) __VA_ARGS__

#define MACRO_RECURSIVE_HELPER_FOR_EACH(Body, ...) \
 __VA_OPT__(MACRO_RECURSIVE_HELPER_EXPAND_0(MACRO_RECURSIVE_HELPER_FOR_EACH_INVOKER(Body, __VA_ARGS__)))

#define MACRO_RECURSIVE_HELPER_FOR_EACH_INVOKER(Body, Arg1, ...) \
 Body(Arg1) \
 __VA_OPT__(MACRO_RECURSIVE_HELPER_FOR_EACH_RETRY MACRO_RECURSIVE_HELPER_PARENTHESES (Body, __VA_ARGS__))

#define MACRO_RECURSIVE_HELPER_FOR_EACH_RETRY() MACRO_RECURSIVE_HELPER_FOR_EACH_INVOKER

#define MACRO_RECURSIVE_FOR_EACH(Body, ...) MACRO_RECURSIVE_HELPER_FOR_EACH(Body __VA_OPT__(, __VA_ARGS__))

#define MACRO_RECURSIVE_HELPER_FOR_EACH_2(Body, ...) \
 __VA_OPT__(MACRO_RECURSIVE_HELPER_EXPAND_0(MACRO_RECURSIVE_HELPER_FOR_EACH_INVOKER_2(Body, __VA_ARGS__)))

#define MACRO_RECURSIVE_HELPER_FOR_EACH_INVOKER_2(Body, Arg1, Arg2, ...) \
 Body(Arg1, Arg2) \
 __VA_OPT__(MACRO_RECURSIVE_HELPER_FOR_EACH_RETRY_2 MACRO_RECURSIVE_HELPER_PARENTHESES (Body, __VA_ARGS__))

#define MACRO_RECURSIVE_HELPER_FOR_EACH_RETRY_2() MACRO_RECURSIVE_HELPER_FOR_EACH_INVOKER_2

#define MACRO_RECURSIVE_FOR_EACH_2(Body, ...) MACRO_RECURSIVE_HELPER_FOR_EACH_2(Body __VA_OPT__(, __VA_ARGS__))

#define MACRO_RECURSIVE_HELPER_FOR_EACH_DOT(Body, ...) \
 __VA_OPT__(MACRO_RECURSIVE_HELPER_EXPAND_0(MACRO_RECURSIVE_HELPER_FOR_EACH_INVOKER_DOT(Body, __VA_ARGS__)))

#define MACRO_RECURSIVE_HELPER_FOR_EACH_INVOKER_DOT(Body, Arg1, ...) \
 Body(Arg1) \
 __VA_OPT__(, MACRO_RECURSIVE_HELPER_FOR_EACH_RETRY_DOT MACRO_RECURSIVE_HELPER_PARENTHESES (Body, __VA_ARGS__))

#define MACRO_RECURSIVE_HELPER_FOR_EACH_RETRY_DOT() MACRO_RECURSIVE_HELPER_FOR_EACH_INVOKER_DOT

#define MACRO_RECURSIVE_FOR_EACH_DOT(Body, ...) MACRO_RECURSIVE_HELPER_FOR_EACH_DOT(Body __VA_OPT__(, __VA_ARGS__))
