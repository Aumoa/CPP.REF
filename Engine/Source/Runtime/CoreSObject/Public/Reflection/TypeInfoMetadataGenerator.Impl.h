// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "TypeInfoMetadataGenerator.h"
#include "Misc/String.h"
#include "typeof.h"
#include <typeinfo>

namespace libty::Core::Reflection
{
	template<class TOwningClass, class... TAttributeCollection>
	TypeInfoMetadataGenerator TypeInfoMetadataGenerator::GenerateClass(std::wstring_view className, std::string_view fullQualifiedClassName, SAssembly* assembly, std::tuple<TAttributeCollection...>& attributes)
	{
		TypeInfoMetadataGenerator gen;
		gen.ClassName = className;
		gen.FullQualifiedClassName = String::AsUnicode(fullQualifiedClassName);
		gen.Assembly = assembly;
		gen.Attributes = MakeAttributeCollection(attributes, std::make_index_sequence<sizeof...(TAttributeCollection)>{});
		gen.Interfaces = MakeInterfaceCollection<TOwningClass>();
		gen.SuperClass = GetSuperClass<TOwningClass>();
		gen.TypeHash = typeid(TOwningClass).hash_code();
		gen.bIsClass = std::derived_from<TOwningClass, SObject>;
		gen.bIsNative = false;
		gen.bIsInterface = false;
		gen.CollectFields<TOwningClass, 0>();
		return gen;
	}

	template<class TOwningClass, class... TAttributeCollection>
	TypeInfoMetadataGenerator TypeInfoMetadataGenerator::GenerateInterface(std::wstring_view interfaceName, std::tuple<TAttributeCollection...>& attributes)
	{
		TypeInfoMetadataGenerator gen;
		gen.ClassName = interfaceName;
		gen.FullQualifiedClassName = interfaceName;
		gen.Assembly = nullptr;
		gen.Attributes = MakeAttributeCollection(attributes, std::make_index_sequence<sizeof...(TAttributeCollection)>{});
		gen.Interfaces = MakeInterfaceCollection<TOwningClass>();
		gen.SuperClass = nullptr;
		gen.TypeHash = typeid(TOwningClass).hash_code();
		gen.bIsClass = true;
		gen.bIsNative = false;
		gen.bIsInterface = true;
		gen.CollectFields<TOwningClass, 0>();
		return gen;
	}

	template<class TNativeClass>
	TypeInfoMetadataGenerator TypeInfoMetadataGenerator::GenerateNative()
	{
		TypeInfoMetadataGenerator gen;
		gen.ClassName = String::AsUnicode(typeid(TNativeClass).name());
		gen.FullQualifiedClassName = gen.ClassName;
		gen.Assembly = nullptr;
		gen.Attributes = {};
		gen.Interfaces = {};
		gen.SuperClass = nullptr;
		gen.TypeHash = typeid(TNativeClass).hash_code();
		gen.bIsClass = false;
		gen.bIsNative = true;
		gen.bIsInterface = false;
		return gen;
	}

	namespace Details
	{
		template<class T>
		concept IInterfaceCollection = requires
		{
			{ std::declval<typename T::InterfaceCollection>() };
		};

		template<class TTuple>
		struct InterfaceCollectionGenerator
		{
		};

		template<class... TInterfaces>
		struct InterfaceCollectionGenerator<std::tuple<TInterfaces...>>
		{
			static std::vector<SType*> Generate()
			{
				std::vector<SType*> list = { TInterfaces::TypeId... };
				static auto less = +[](SType* lhs, SType* rhs) { return lhs->GetHashCode() < rhs->GetHashCode(); };
				std::sort(list.begin(), list.end(), less);
				static auto equals = +[](SType* lhs, SType* rhs) { return lhs->GetHashCode() == rhs->GetHashCode(); };
				auto unique_end = std::unique(list.begin(), list.end(), equals);
				list.erase(unique_end, list.end());
				return list;
			}
		};
	}

	template<class TOwningClass>
	std::vector<SType*> TypeInfoMetadataGenerator::MakeInterfaceCollection()
	{
		if constexpr (Details::IInterfaceCollection<TOwningClass>)
		{
			return Details::InterfaceCollectionGenerator<typename TOwningClass::InterfaceCollection>::Generate();
		}
		else
		{
			return {};
		}
	}

	template<class TOwningClass>
	SType* TypeInfoMetadataGenerator::GetSuperClass() requires
		requires { std::declval<typename TOwningClass::Super>(); } &&
		(not std::same_as<typename TOwningClass::Super, void>)
	{
		return typeof(typename TOwningClass::Super);
	}

	template<class TOwningClass>
	SType* TypeInfoMetadataGenerator::GetSuperClass()
	{
		return nullptr;
	}
	
	template<class TOwningClass>
	void TypeInfoMetadataGenerator::GenerateConstructor()
	{
		if constexpr (std::constructible_from<TOwningClass>)
		{
			if constexpr (std::derived_from<TOwningClass, SObject>)
			{
				Constructor = []() { return gcnew TOwningClass(); };
			}
			else
			{
				Constructor = []() { return Cast<SObject>(TOwningClass()); };
			}
		}
	}
}