// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <vector>
#include <functional>

class SObject;
class SAttributeClass;
class SFieldInfo;
class SAssembly;

namespace libty::Core::Reflection
{
	struct TypeInfoMetadataGenerator
	{
		using ClassAttributeCollection = std::vector<SAttributeClass*>;
		using ClassFieldsCollection = std::vector<SFieldInfo*>;
		using InterfaceCollection = std::vector<SType*>;

		// ** Common metadata **
		std::wstring ClassName;
		std::wstring FullQualifiedClassName;
		SAssembly* Assembly;
		ClassAttributeCollection Attributes;
		InterfaceCollection Interfaces;
		SType* SuperClass;
		size_t TypeHash;
		std::function<SObject* ()> Constructor;
		uint8 bIsNative : 1;
		uint8 bIsClass : 1;
		uint8 bIsInterface : 1;

		// ** Linked metadata **
		ClassFieldsCollection Fields;

		template<class TOwningClass, class... TAttributeCollection>
		static TypeInfoMetadataGenerator GenerateClass(std::wstring_view className, std::string_view fullQualifiedClassName, SAssembly* assembly, std::tuple<TAttributeCollection...>& attributes);

		template<class TInterface, class... TAttributeCollection>
		static TypeInfoMetadataGenerator GenerateInterface(std::wstring_view interfaceName, std::tuple<TAttributeCollection...>& attributes);

		template<class TNativeClass>
		static TypeInfoMetadataGenerator GenerateNative();

	private:
		template<class... TAttributeCollection, size_t... Idx>
		static std::vector<SAttributeClass*> MakeAttributeCollection(std::tuple<TAttributeCollection...>& attributes, std::index_sequence<Idx...>&&)
		{
			return std::vector<SAttributeClass*>{ (&std::get<Idx>(attributes))... };
		}

		template<class TOwningClass>
		static std::vector<SType*> MakeInterfaceCollection();

		template<class TOwningClass>
		static SType* GetSuperClass() requires
			requires { std::declval<typename TOwningClass::Super>(); } &&
			(not std::same_as<typename TOwningClass::Super, void>);

		template<class TOwningClass>
		static SType* GetSuperClass();

		template<class TOwningClass, size_t N>
		void CollectFields()
		{
			if constexpr (std::same_as<decltype(TOwningClass::template REFLECTION_GetPropertyPointer<N>(0)), void>)
			{
				return;
			}
			else
			{
				Fields.emplace_back(TOwningClass::template REFLECTION_GetPropertyPointer<N>(0));
				CollectFields<TOwningClass, N + 1>();
			}
		}

		template<class TOwningClass>
		void GenerateConstructor();
	};
}