// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <vector>
#include <functional>

namespace libty::inline Core
{
	class SObject;

	inline namespace Attributes
	{
		class SClassAttribute;
	}
}

namespace libty::inline Core::Reflection
{
	class SFieldInfo;
	class SMethodInfo;
	class SAssembly;

	template<class TOwningClass>
	concept IInternalAccessModifierIsPublic = requires
	{
		{ TOwningClass::__INTERNAL_AccessModifierChecker() } -> std::same_as<int32>;
	};

	template<char C>
	struct is_class_name : public std::bool_constant<C == 'S'>
	{
	};

	template<char C>
	struct is_interface_name : public std::bool_constant<C == 'I'>
	{
	};

	template<char C>
	struct is_struct_name : public std::bool_constant<true>
	{
	};

	template<char C>
	struct is_enum_name : public std::bool_constant<C == 'E'>
	{
	};

	struct TypeInfoMetadataGenerator
	{
		using ClassAttributeCollection = std::vector<SClassAttribute*>;
		using ClassFieldsCollection = std::vector<SFieldInfo*>;
		using ClassMethodsCollection = std::vector<SMethodInfo*>;
		using InterfaceCollection = std::vector<SType*>;
		using TypeGetter = std::function<SType* ()>;

		// ** Common metadata **
		String ClassName;
		String FullQualifiedClassName;
		SAssembly* Assembly;
		ClassAttributeCollection Attributes;
		InterfaceCollection Interfaces;
		TypeGetter SuperClass;
		size_t TypeHash;
		std::function<SObject* ()> Constructor;
		uint8 bIsNative : 1;
		uint8 bIsValueType : 1;
		uint8 bIsInterface : 1;
		uint8 bIsEnum : 1;

		// ** Linked metadata **
		ClassFieldsCollection Fields;
		ClassMethodsCollection Methods;

		// ** Enum Specialization **
		using EnumTryParseObjFunc = bool(*)(StringView, SObject*&);
		EnumTryParseObjFunc EnumTryParseObj;
		using EnumTryParseFunc = bool(*)(StringView, int64&);
		EnumTryParseFunc EnumTryParse;

		template<char ClassType, class TOwningClass, class... TAttributeCollection>
		static TypeInfoMetadataGenerator GenerateManaged
		(
			StringView className,
			StringView fullQualifiedClassName,
			SAssembly* assembly,
			std::tuple<TAttributeCollection...>& attributes
		);

		template<class TNativeClass>
		static TypeInfoMetadataGenerator GenerateNative();

	private:
		template<class TOwningClass, class... TAttributeCollection, size_t... Idx>
		static std::vector<SClassAttribute*> MakeAttributeCollection(std::tuple<TAttributeCollection...>& attributes, std::index_sequence<Idx...>&&);

		template<class TOwningClass>
		static std::vector<SType*> MakeInterfaceCollection();

		template<class TOwningClass>
		static TypeGetter GetSuperClass() requires
			requires { std::declval<typename TOwningClass::Super>(); } &&
		(not std::same_as<typename TOwningClass::Super, void>);

		template<class TOwningClass>
		static TypeGetter GetSuperClass();

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

		template<class TOwningClass, size_t N>
		void CollectMethods()
		{
			if constexpr (std::same_as<decltype(TOwningClass::template REFLECTION_GetFunctionPointer<N>(0)), void>)
			{
				return;
			}
			else
			{
				Methods.emplace_back(TOwningClass::template REFLECTION_GetFunctionPointer<N>(0));
				CollectMethods<TOwningClass, N + 1>();
			}
		}

		template<class TOwningClass>
		void GenerateConstructor();
	};
}