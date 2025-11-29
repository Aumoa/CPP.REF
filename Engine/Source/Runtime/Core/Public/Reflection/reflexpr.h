// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"

namespace std::experimental::reflect
{
	template<class T>
	struct get_aliased
	{
	};

	template<class T>
	struct get_data_members
	{
	};

	template<class T>
	struct get_name
	{
	};

	template<class T>
	struct get_scope
	{
	};

	template<class T>
	struct get_field_type
	{
	};

	template<class T>
	struct get_field_offset
	{
	};

	template<class T>
	struct get_access
	{
	};

	
	enum class access_type
	{
		public_,
		protected_,
		private_
	};


	struct reflexpr_aliased
	{
		using is_reflexpr_aliased = int;
	};

	template<class T>
	concept is_reflexpr_aliased = std::same_as<typename T::is_reflexpr_aliased, int>;

	template<is_reflexpr_aliased T>
	struct get_name<T>
	{
		static constexpr ::Ayla::String value = T::name;
	};

	template<is_reflexpr_aliased T>
	struct get_scope<T>
	{
		static constexpr ::Ayla::String value = T::scope;
	};


	template<access_type access_>
	struct reflexpr_member
	{
		using is_reflexpr_member = int;
		static constexpr access_type access = access_;
	};

	template<class T>
	concept is_reflexpr_member = std::same_as<typename T::is_reflexpr_member, int>;

	template<is_reflexpr_member T>
	struct get_access<T>
	{
		static constexpr access_type value = T::access;
	};


	template<access_type access_, class T, size_t Off>
	struct reflexpr_field : public reflexpr_member<access_>
	{
		using is_reflexpr_field = int;
		using field_type = T;
		static constexpr size_t offset = Off;
	};

	template<class T>
	concept is_reflexpr_field = std::same_as<typename T::is_reflexpr_field, int>;

	template<is_reflexpr_field T>
	struct get_field_type<T>
	{
		using type = T::field_type;;
	};

	template<is_reflexpr_field T>
	struct get_field_offset<T>
	{
		static constexpr size_t value = T::offset;
	};


	template<access_type access_, class T, T Pointer>
	struct reflexpr_method : public reflexpr_member<access_>
	{
		using is_reflexpr_method = int;
		using function_type = T;
		static constexpr T pointer = Pointer;
	};

	template<class T>
	concept is_reflexpr_method = std::same_as<typename T::is_reflexpr_method, int>;

	
	template<access_type access_, class T, T Pointer>
	struct reflexpr_constructor : public reflexpr_member<access_>
	{
		using is_reflexpr_constructor = int;
		using function_type = T;
		static constexpr T pointer = Pointer;
	};

	template<class T>
	concept is_reflexpr_constructor = std::same_as<typename T::is_reflexpr_constructor, int>;


	struct reflexpr_members
	{
		using is_reflexpr_members = int;
	};

	template<class T>
	concept is_reflexpr_members = std::same_as<typename T::is_reflexpr_members, int>;

	template<class T, class aliased = typename T::aliased, class members = typename T::members>
	concept is_reflexpr_class = requires
	{
		{ aliased::name } -> std::convertible_to<::Ayla::String>;
		{ aliased::scope } -> std::convertible_to<::Ayla::String>;
	};

	template<is_reflexpr_class T>
	struct get_aliased<T>
	{
		using type = T::aliased;
	};

	template<is_reflexpr_class T>
	struct get_data_members<T>
	{
		using type = T::members;
	};


	template<class T, typename get_aliased<T>::type* = nullptr>
	using get_aliased_t = typename get_aliased<T>::type;

	template<class T, typename get_data_members<T>::type* = nullptr>
	using get_data_members_t = typename get_data_members<T>::type;

	template<class T, typename get_field_type<T>::type* = nullptr>
	using get_field_type_t = typename get_field_type<T>::type;

	template<class T> requires requires { { get_access<T>::value } -> std::convertible_to<access_type>; }
	constexpr access_type get_access_v = get_access<T>::value;

	template<class T> requires requires { { T::name } -> std::same_as<::Ayla::String>; }
	constexpr ::Ayla::String get_name_v = T::name;

	template<class T> requires requires { { T::scope } -> std::same_as<Ayla::String>; }
	constexpr ::Ayla::String get_scope_v = T::scope;

	template<class T> requires requires { { get_field_offset<T>::value } -> std::convertible_to<size_t>; }
	constexpr size_t get_field_offset_v = get_field_offset<T>::value;
}

namespace std
{
	template<experimental::reflect::is_reflexpr_members T>
	struct tuple_size<T> : public integral_constant<size_t, T::tuple_size>
	{
	};

	template<size_t N, experimental::reflect::is_reflexpr_members T>
	consteval auto get(const T&)
	{
		return T::template get<N>();
	}

	template<size_t I, experimental::reflect::is_reflexpr_members T>
	struct tuple_element<I, T>
	{
		using type = decltype(T::template get<I>());
	};
}