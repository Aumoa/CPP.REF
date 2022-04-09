// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "TypeInfoMetadataGenerator.h"
#include "typeof.h"
#include "Enum.h"
#include "Attributes/ClassAttribute.h"
#include <typeinfo>

namespace libty::inline Core::Reflection
{
	template<char ClassType, class TOwningClass, class... TAttributeCollection>
	TypeInfoMetadataGenerator TypeInfoMetadataGenerator::GenerateManaged
	(
		std::wstring_view className,
		std::wstring_view fullQualifiedClassName,
		SAssembly* assembly,
		std::tuple<TAttributeCollection...>& attributes
	)
	{
		constexpr bool IsObject = std::derived_from<TOwningClass, SObject>;
		constexpr bool IsInterface = IsObject && IInternalAccessModifierIsPublic<TOwningClass>;
		constexpr bool IsEnum = IEnum<TOwningClass>;
		constexpr bool IsClass = IsObject && !IsInterface;
		constexpr bool IsStruct = !IsObject && !IsEnum;

		static_assert(!IsClass || is_class_name<ClassType>::value,
			"Unsupported first character for class name. See libty::Core::Reflection::is_class_name.");
		static_assert(!IsInterface || is_interface_name<ClassType>::value,
			"Unsupported first character for interface name. See libty::Core::Reflection::is_interface_name.");
		static_assert(!IsStruct || is_struct_name<ClassType>::value,
			"Unsupported first character for struct name. See libty::Core::Reflection::is_struct_name.");
		static_assert(!IsEnum || is_enum_name<ClassType>::value,
			"Unsupported first character for enum name. See libty::Core::Reflection::is_enum_name.");

		TypeInfoMetadataGenerator gen;
		gen.ClassName = className;
		gen.FullQualifiedClassName = fullQualifiedClassName;
		gen.Assembly = assembly;
		gen.Attributes = MakeAttributeCollection<TOwningClass>(attributes, std::make_index_sequence<sizeof...(TAttributeCollection)>{});
		gen.Interfaces = MakeInterfaceCollection<TOwningClass>();
		gen.SuperClass = GetSuperClass<TOwningClass>();
		gen.TypeHash = typeid(TOwningClass).hash_code();
		gen.bIsNative = false;
		gen.bIsValueType = !IsObject;
		gen.bIsInterface = IsInterface;
		gen.bIsEnum = !IsObject && IsEnum;
		gen.CollectFields<TOwningClass, 0>();
		gen.CollectMethods<TOwningClass, 0>();
		gen.GenerateConstructor<TOwningClass>();

		if constexpr (IsEnum)
		{
			gen.EnumTryParseObj = +[](std::wstring_view format, SObject*& value) -> bool
			{
				TOwningClass ov;
				bool result = TOwningClass::TryParse(format, ov);
				if (!result)
				{
					value = nullptr;
					return false;
				}

				value = Cast<SObject>(ov);
				return true;
			};

			gen.EnumTryParse = +[](std::wstring_view format, int64& value) -> bool
			{
				TOwningClass ov;
				bool result = TOwningClass::TryParse(format, ov);
				if (!result)
				{
					return false;
				}

				value = (int64)ov.Value;
				return true;
			};
		}

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
		gen.bIsNative = true;
		gen.bIsValueType = true;
		gen.bIsInterface = false;
		gen.bIsEnum = false;
		return gen;
	}

	template<class TOwningClass, class... TAttributeCollection, size_t... Idx>
	std::vector<SClassAttribute*> TypeInfoMetadataGenerator::MakeAttributeCollection(std::tuple<TAttributeCollection...>& attributes, std::index_sequence<Idx...>&&)
	{
		auto collection = std::vector<SClassAttribute*>{ (&std::get<Idx>(attributes))... };

		for (auto& item : collection)
		{
			item->SetBindType(TOwningClass::TypeId);
		}
		return collection;
	}

	namespace Details
	{
		template<class T>
		concept IInterfaceCollection = requires
		{
			{ std::declval<typename T::InterfaceCollection*>() };
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
				std::sort(list.begin(), list.end());
				auto unique_end = std::unique(list.begin(), list.end());
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