// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "TypeInfoMetadataGenerator.h"
#include "Misc/String.h"
#include "typeof.h"
#include <typeinfo>

namespace libty::Core::Reflection
{
	template<class TOwningClass, class... TAttributeCollection>
	static TypeInfoMetadataGenerator TypeInfoMetadataGenerator::GenerateClass(std::wstring_view className, std::string_view fullQualifiedClassName, const std::tuple<TAttributeCollection...>& attributes)
	{
		TypeInfoMetadataGenerator gen;
		gen.ClassName = className;
		gen.FullQualifiedClassName = String::AsUnicode(fullQualifiedClassName);
		gen.Attributes = MakeVectorCollection(attributes, std::make_index_sequence<sizeof...(TAttributeCollection)>{});
		gen.SuperClass = GetSuperClass<TOwningClass>();
		gen.TypeHash = typeid(TOwningClass).hash_code();
		gen.bIsClass = true;
		gen.bIsNative = false;
		gen.CollectFields<TOwningClass, 0>();
		return gen;
	}

	template<class TNativeClass>
	static TypeInfoMetadataGenerator TypeInfoMetadataGenerator::GenerateNative()
	{
		TypeInfoMetadataGenerator gen;
		gen.ClassName = String::AsUnicode(typeid(TNativeClass).name());
		gen.FullQualifiedClassName = gen.ClassName;
		gen.Attributes = {};
		gen.SuperClass = nullptr;
		gen.TypeHash = typeid(TNativeClass).hash_code();
		gen.bIsClass = false;
		gen.bIsNative = true;
		return gen;
	}

	template<class TOwningClass>
	static SType* TypeInfoMetadataGenerator::GetSuperClass() requires
		requires { std::declval<typename TOwningClass::Super>(); } &&
		(not std::same_as<typename TOwningClass::Super, void>)
	{
		return typeof(typename TOwningClass::Super);
	}

	template<class TOwningClass>
	static SType* TypeInfoMetadataGenerator::GetSuperClass()
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