// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <type_traits>

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
