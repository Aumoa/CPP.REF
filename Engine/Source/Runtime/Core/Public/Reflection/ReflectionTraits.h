// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "AccessModifier.h"
#include <vector>

class Object;
class Type;

namespace libty::reflect
{
	template<class T>
	inline Type* get_class() requires
		std::is_pointer_v<T>&&
		std::derived_from<std::remove_pointer_t<T>, Object>
	{
		return std::remove_pointer_t<T>::InstantiatedClass();
	}

	template<class T>
	inline Type* get_class()
	{
		return nullptr;
	}

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

		String name;
		getter_t getter;
		setter_t setter;
		Type* reflect_type;
		EAccessModifier access_modifier;

		template<class T>
		static inline property_info_t generate(const String& name, getter_t getter, setter_t setter, EAccessModifier access_modifier)
		{
			property_info_t p;
			p.name = name;
			p.getter = getter;
			p.setter = setter;
			p.reflect_type = get_class<T>();
			p.access_modifier = access_modifier;
			return p;
		}
	};

	struct function_info_t
	{
		using function_t = void*(*)(void*, std::vector<void*>);

		String name;
		function_t fnc;
		size_t hash;
		Type* return_type;
		std::vector<Type*> argument_types;

		template<class T, class TReturnType, class... TArgs>
		static inline function_info_t generate(const String& name, function_t fnc, size_t hash)
		{
			function_info_t p;
			p.name = name;
			p.fnc = fnc;
			p.hash = hash;
			p.return_type = get_class<TReturnType>();
			p.argument_types = std::vector<Type*>{ get_class<TArgs>()... };
			return p;
		}

		inline operator function_t() const
		{
			return fnc;
		}
	};
}