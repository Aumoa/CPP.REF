// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "AccessModifier.h"
#include <vector>

class Object;
class Type;

namespace libty::reflect
{
	struct constructor_t
	{
		using function_t = void*(*)(std::vector<void*>);

		function_t fnc;
		size_t hash;
		bool is_gc;

		inline constructor_t(function_t fnc, size_t hash, bool is_gc)
			: fnc(fnc)
			, hash(hash)
			, is_gc(is_gc)
		{
		}

		inline operator function_t() const
		{
			return fnc;
		}
	};

	struct property_info_t
	{
		using getter_t = void*(*)(void*);
		using setter_t = void(*)(void*, void*);

		getter_t getter;
		setter_t setter;
		Type* reflect_type;
		EAccessModifier access_modifier;

		template<class T>
		static inline property_info_t generate(getter_t getter, setter_t setter, EAccessModifier access_modifier)
		{
			property_info_t p;
			p.getter = getter;
			p.setter = setter;
			p.reflect_type = get_class<T>();
			p.access_modifier = access_modifier;
			return p;
		}

		template<class T>
		static inline Type* get_class() requires
			std::is_pointer_v<T> &&
			std::derived_from<std::remove_pointer_t<T>, Object>
		{
			return std::remove_pointer_t<T>::StaticClass();
		}

		template<class T>
		static inline Type* get_class()
		{
			return nullptr;
		}
	};

	struct function_info_t
	{
		using function_t = void*(*)(std::vector<void*>);

		function_t fnc;
		String name;
		size_t hash;

		inline function_info_t(function_t fnc, const String& name, size_t hash)
			: fnc(fnc)
			, name(name)
			, hash(hash)
		{
		}

		inline operator function_t() const
		{
			return fnc;
		}
	};
}