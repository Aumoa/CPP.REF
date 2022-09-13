// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "reflexpr.h"
#include "Misc/String.h"
#include "Reflection/ReflectionTraits.h"
#include <memory>

class Type;
class Object;

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
		bool bGen = false;
		int32 ClassType;
		size_t HashCode;
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
			bool bIsGC = false;
			if constexpr (libty::reflect::is_class<T>)
			{
				M.ClassType = 0;
				bIsGC = true;
			}
			else if constexpr (libty::reflect::is_interface<T>)
			{
				M.ClassType = 2;
				bIsGC = true;
			}
			M.HashCode = typeid(typename T::type_t).hash_code();
			M.FriendlyName = T::friendly_name();
			M.Constructors = T::constructors();
			M.Properties = T::properties();
			M.Functions = T::functions();
			if constexpr (std::constructible_from<typename T::type_t>)
			{
				// Supports default constructor.
				M.Constructors.emplace(M.Constructors.begin(), constructor_t([](std::vector<void*>) -> void*
				{
					using InstanceType = typename T::type_t;
					return dynamic_cast<Object*>(new InstanceType());
				}, 0, bIsGC));
			}
			if constexpr (libty::reflect::is_class<typename T::super_t>)
			{
				using super_t = typename T::super_t;
				using type_t = typename super_t::type_t;
				M.Super = type_t::InstantiatedClass();
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