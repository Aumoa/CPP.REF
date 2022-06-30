// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "reflexpr.h"
#include "Misc/String.h"
#include "Reflection/ReflectionTraits.h"
#include <memory>

class Type;

namespace libty::reflect
{
	template<class T>
	struct GenerateInterfaces
	{
	};

	template<class T, class... TArgs>
	struct GenerateInterfaces<std::tuple<T, TArgs...>>
	{
		static void Generate(struct ClassTypeMetadata& M);
	};

	template<>
	struct GenerateInterfaces<std::tuple<>>
	{
		static void Generate(struct ClassTypeMetadata& M)
		{
		}
	};

	struct ClassTypeMetadata
	{
		String FriendlyName;
		std::vector<constructor_t> Constructors;
		std::vector<property_info_t> Properties;
		std::vector<function_info_t> Functions;
		Type* Super = nullptr;
		std::vector<Type*> Interfaces;

		template<class T>
		static ClassTypeMetadata Generate()
		{
			ClassTypeMetadata M;
			M.FriendlyName = get_friendly_name_v<T>;
			M.Constructors = T::constructors;
			M.Properties = T::properties;
			M.Functions = T::functions;
			if constexpr (libty::reflect::is_class<typename T::super_t>)
			{
				using super_t = typename T::super_t;
				using type_t = typename super_t::type_t;
				M.Super = type_t::StaticClass();
			}
			libty::reflect::GenerateInterfaces<typename T::interfaces_t>::Generate(M);
			return M;
		}

		template<class T>
		static void GenerateInterfaces(ClassTypeMetadata&);
		template<class... TArgs>
		static void GenerateInterfaces(ClassTypeMetadata& M);
	};

	template<class T, class... TArgs>
	void GenerateInterfaces<std::tuple<T, TArgs...>>::Generate(ClassTypeMetadata& M)
	{
		auto& ptr = M.Interfaces.emplace_back();
		ptr = T::StaticClass();
		if constexpr (sizeof...(TArgs) > 0)
		{
			GenerateInterfaces<TArgs...>::Generate(M);
		}
	}
}