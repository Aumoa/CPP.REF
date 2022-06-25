// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include "Concepts/Variadic.h"
#include "Concepts/GetVariadic.h"
#include "Reflection/ReflectionTraits.h"
#include <concepts>

#define reflexpr(Class) libty::reflect::reflexpr_ ## Class



namespace libty::reflect
{
	struct reflexpr_void
	{
		using super_t = void;
		static constexpr String friendly_name = TEXT("void");
		static inline std::vector<constructor_t> constructors;
		static inline std::vector<property_info_t> properties;
	};

	template<class T>
	inline constexpr String get_friendly_name_v = T::friendly_name;



	template<class T>
	concept is_class = std::same_as<typename T::is_class_t, int>;

	template<class T>
	concept is_primitive =
		std::integral<T> ||
		std::floating_point<T>;

	template<class T>
	concept is_interface = std::same_as<typename T::is_interface_t, int>;

	template<class T>
	concept is_enum = std::same_as<typename T::is_enum_t, int> || std::is_enum_v<T>;

	template<class T>
	concept is_struct = 
		std::same_as<typename T::is_struct_t, int> ||
		(!is_primitive<T> && !is_class<T> && !is_interface<T> && !is_enum<T>);



	template<class T>
	concept is_public = std::same_as<typename T::is_public_t, int>;

	template<class T>
	concept is_private = std::same_as<typename T::is_private_t, int>;

	template<class T>
	concept is_protected = std::same_as<typename T::is_protected_t, int>;

	template<class T>
	concept is_readonly = std::same_as<typename T::is_readonly_t, int>;
}