// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <type_traits>
#include <optional>
#include <functional>

#define GENERATE_BITMASK_ENUM_OPERATORS_FRIEND(ClassType) \
friend inline constexpr ClassType operator |(ClassType Lhs, ClassType Rhs) noexcept \
{ \
	using T = std::underlying_type_t<ClassType>; \
	return (ClassType)((T)Lhs | (T)Rhs); \
} \
\
friend inline constexpr ClassType operator &(ClassType Lhs, ClassType Rhs) noexcept \
{ \
	using T = std::underlying_type_t<ClassType>; \
	return (ClassType)((T)Lhs & (T)Rhs); \
} \
\
friend inline constexpr ClassType& operator |=(ClassType& Lhs, ClassType Rhs) noexcept \
{ \
	Lhs = Lhs | Rhs; \
	return Lhs; \
} \
\
friend inline constexpr ClassType& operator &=(ClassType& Lhs, ClassType Rhs) noexcept \
{ \
	Lhs = Lhs & Rhs; \
	return Lhs; \
} \
\
friend inline constexpr ClassType operator ~(ClassType u) noexcept \
{ \
	using T = std::underlying_type_t<ClassType>; \
	return (ClassType)~(T)u; \
} \
\
friend inline constexpr bool operator ==(ClassType Lhs, std::underlying_type_t<ClassType> Rhs) noexcept \
{ \
	return static_cast<std::underlying_type_t<ClassType>>(Lhs) == Rhs; \
} \
\
friend inline constexpr auto operator <=>(ClassType Lhs, std::underlying_type_t<ClassType> Rhs) noexcept \
{ \
	return static_cast<std::underlying_type_t<ClassType>>(Lhs) <=> Rhs; \
}

#define GENERATE_BITMASK_ENUM_OPERATORS(ClassType) \
inline constexpr ClassType operator |(ClassType Lhs, ClassType Rhs) noexcept \
{ \
	using T = std::underlying_type_t<ClassType>; \
	return (ClassType)((T)Lhs | (T)Rhs); \
} \
\
inline constexpr ClassType operator &(ClassType Lhs, ClassType Rhs) noexcept \
{ \
	using T = std::underlying_type_t<ClassType>; \
	return (ClassType)((T)Lhs & (T)Rhs); \
} \
\
inline constexpr ClassType& operator |=(ClassType& Lhs, ClassType Rhs) noexcept \
{ \
	Lhs = Lhs | Rhs; \
	return Lhs; \
} \
\
inline constexpr ClassType& operator &=(ClassType& Lhs, ClassType Rhs) noexcept \
{ \
	Lhs = Lhs & Rhs; \
	return Lhs; \
} \
\
inline constexpr ClassType operator ~(ClassType u) noexcept \
{ \
	using T = std::underlying_type_t<ClassType>; \
	return (ClassType)~(T)u; \
} \
\
inline constexpr bool operator ==(ClassType Lhs, std::underlying_type_t<ClassType> Rhs) noexcept \
{ \
	return static_cast<std::underlying_type_t<ClassType>>(Lhs) == Rhs; \
} \
\
inline constexpr auto operator <=>(ClassType Lhs, std::underlying_type_t<ClassType> Rhs) noexcept \
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

namespace Ayla::LaunguageSupportMacros
{
	template<class Mtx>
	class lock_append
	{
		std::optional<Mtx> m_Mtx;
		bool m_Dis;

	public:
		inline lock_append(Mtx&& mtx) noexcept
			: m_Mtx(std::move(mtx))
		{
		}

		inline ~lock_append() noexcept
		{
			m_Mtx.reset();
		}

		inline void operator <<(std::function<void(Mtx&)> body)
		{
			body(m_Mtx.value());
			m_Mtx.reset();
		}

		template<class U>
		inline U operator <<(std::function<U(Mtx&)> body)
		{
			U r = body(m_Mtx.value());
			m_Mtx.reset();
			return r;
		}

		template<class L>
		inline auto operator <<(L&& body) -> std::invoke_result_t<L, Mtx>
			requires std::invocable<L, Mtx&>
		{
			return *this * (std::function<std::invoke_result_t<L, Mtx>(Mtx&)>)body;
		}
	};
}

#define al_lock(Mtx) ::Ayla::LaunguageSupportMacros::lock_append(std::unique_lock(Mtx)) << [&](std::unique_lock<std::remove_reference_t<decltype(Mtx)>>& lock_)
