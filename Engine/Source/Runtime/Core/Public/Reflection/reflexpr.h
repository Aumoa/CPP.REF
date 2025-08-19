// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"

namespace std::experimental::reflect
{
	template<class T, class aliased = typename T::aliased, class members = typename T::members>
	concept Object = requires
	{
		{ aliased::name } -> std::convertible_to<::Ayla::String>;
		{ aliased::scope } -> std::convertible_to<::Ayla::String>;
	};

	struct reflexpr_member
	{
	};

	template<class T>
	struct reflexpr_field : public reflexpr_member
	{
		using field_type_t = T;
	};

	template<class T>
	using get_aliased_t = T::aliased;

	template<class T>
	using get_data_members_t = T::members;

	template<class T>
	constexpr ::Ayla::String get_name_v = T::name;

	template<class T>
	constexpr ::Ayla::String get_scope_v = T::scope;
}

namespace std
{
	template<class T> requires requires(const T& t) { { t.tuple_size } -> convertible_to<size_t>; }
	struct tuple_size<T> : public integral_constant<size_t, T::tuple_size>
	{
	};

	template<size_t N, class T> requires requires { { T::template get<N>() }; }
	consteval auto get(const T&)
	{
		return T::template get<N>();
	}
}