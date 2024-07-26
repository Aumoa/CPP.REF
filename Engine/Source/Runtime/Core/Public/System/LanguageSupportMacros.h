// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <type_traits>

#define GENERATE_BITMASK_ENUM_OPERATORS(ClassType, ExportPrefix) \
ExportPrefix inline constexpr ClassType operator |(ClassType Lhs, ClassType Rhs) noexcept \
{ \
	using T = std::underlying_type_t<ClassType>; \
	return (ClassType)((T)Lhs | (T)Rhs); \
} \
\
ExportPrefix inline constexpr ClassType operator &(ClassType Lhs, ClassType Rhs) noexcept \
{ \
	using T = std::underlying_type_t<ClassType>; \
	return (ClassType)((T)Lhs & (T)Rhs); \
} \
\
ExportPrefix inline constexpr ClassType& operator |=(ClassType& Lhs, ClassType Rhs) noexcept \
{ \
	Lhs = Lhs | Rhs; \
	return Lhs; \
} \
\
ExportPrefix inline constexpr ClassType& operator &=(ClassType& Lhs, ClassType Rhs) noexcept \
{ \
	Lhs = Lhs & Rhs; \
	return Lhs; \
} \
\
ExportPrefix inline constexpr ClassType operator ~(ClassType u) noexcept \
{ \
	using T = std::underlying_type_t<ClassType>; \
	return (ClassType)~(T)u; \
} \
\
ExportPrefix inline constexpr bool operator ==(ClassType Lhs, std::underlying_type_t<ClassType> Rhs) noexcept \
{ \
	return static_cast<std::underlying_type_t<ClassType>>(Lhs) == Rhs; \
} \
\
ExportPrefix inline constexpr auto operator <=>(ClassType Lhs, std::underlying_type_t<ClassType> Rhs) noexcept \
{ \
	return static_cast<std::underlying_type_t<ClassType>>(Lhs) <=> Rhs; \
}

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


#define TEXT(X) (String::FromLiteral(L ## X))
#define DECLARE_MULTICAST_DELEGATE(DelegateName, ...) using DelegateName = MulticastDelegate<void(__VA_ARGS__)>
