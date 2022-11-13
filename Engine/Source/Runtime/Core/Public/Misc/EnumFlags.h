// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#define DEFINE_ENUM_FLAGS_OPERATORS(Name) \
template<class TEnum>\
inline constexpr TEnum operator |(const TEnum& lhs, const TEnum& rhs) noexcept requires std::same_as<TEnum, Name>\
{\
	using T = std::underlying_type_t<TEnum>;\
	return (TEnum)((T)lhs | (T)rhs);\
}\
\
template<class TEnum>\
inline constexpr TEnum operator &(const TEnum& lhs, const TEnum& rhs) noexcept requires std::same_as<TEnum, Name>\
{\
	using T = std::underlying_type_t<TEnum>;\
	return (TEnum)((T)lhs & (T)rhs);\
}\
\
template<class TEnum>\
inline constexpr TEnum operator ^(const TEnum& lhs, const TEnum& rhs) noexcept requires std::same_as<TEnum, Name>\
{\
	using T = std::underlying_type_t<TEnum>;\
	return (TEnum)((T)lhs ^ (T)rhs);\
}\
\
template<class TEnum>\
inline constexpr TEnum& operator |=(TEnum& lhs, const TEnum& rhs) noexcept requires std::same_as<TEnum, Name>\
{\
	return lhs = lhs | rhs;\
}\
\
template<class TEnum>\
inline constexpr TEnum& operator &=(TEnum& lhs, const TEnum& rhs) noexcept requires std::same_as<TEnum, Name>\
{\
	return lhs = lhs & rhs;\
}\
\
template<class TEnum>\
inline constexpr TEnum& operator ^=(TEnum& lhs, const TEnum& rhs) noexcept requires std::same_as<TEnum, Name>\
{\
	return lhs = lhs ^ rhs;\
}\
\
template<class TEnum>\
inline constexpr TEnum operator ~(const TEnum& lhs) noexcept requires std::same_as<TEnum, Name>\
{\
	using T = std::underlying_type_t<TEnum>;\
	return (TEnum)(~(T)lhs);\
}