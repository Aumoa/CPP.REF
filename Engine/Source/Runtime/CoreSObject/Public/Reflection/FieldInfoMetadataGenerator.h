// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "ObjectCasts.h"
#include <vector>
#include <functional>
#include <any>

namespace libty::inline Core
{
	class SObject;
	class SType;

	inline namespace Attributes
	{
		class SFieldAttribute;
	}

	namespace Reflection
	{
		struct FieldInfoMetadataGenerator
		{
			using SetterFunc = std::function<void(SObject* _This, SObject* _Value)>;
			using GetterFunc = std::function<SObject* (SObject* _This)>;
			using GetterFuncAny = std::function<std::any(SObject* _This)>;
			using FieldAttributeCollection = std::vector<SFieldAttribute*>;
			using NativeGCInvoke = std::function<void(SObject* _Obj)>;
			using NativeGCCollection = std::function<void(FieldInfoMetadataGenerator*, SObject*, NativeGCInvoke&)>;

			String FieldName;
			FieldAttributeCollection Attributes;
			uint8 bIsConst : 1;
			uint8 bIsStatic : 1;

			SetterFunc Setter;
			GetterFunc Getter;
			GetterFuncAny GetterAny;
			SType* FieldType;
			NativeGCCollection Collection;

			template<class TMemberType, class TOwningClass, class... TAttributeCollection>
			FieldInfoMetadataGenerator(TMemberType TOwningClass::* field, String fieldName, std::tuple<TAttributeCollection...>& attributes);

			template<class TMemberType, class... TAttributeCollection>
			FieldInfoMetadataGenerator(TMemberType* field, String fieldName, std::tuple<TAttributeCollection...>& attributes);

			template<class TMemberType, class TOwningClass, class... TAttributeCollection>
			FieldInfoMetadataGenerator(const TMemberType TOwningClass::* field, String fieldName, std::tuple<TAttributeCollection...>& attributes);

			template<class TMemberType, class... TAttributeCollection>
			FieldInfoMetadataGenerator(const TMemberType* field, String fieldName, std::tuple<TAttributeCollection...>& attributes);

		private:
			template<class TMemberType>
			void SupportNativeObjectCollection(InheritSelector<2>&&) requires
				IEnumerable<TMemberType, SObject*>;

			template<class TMemberType>
			void SupportNativeObjectCollection(InheritSelector<2>&&) requires
				IDictionary<TMemberType, SObject*, SObject*>;

			template<class TMemberType>
			void SupportNativeObjectCollection(InheritSelector<1>&&) requires
				IDictionary<TMemberType, DictionaryKey_t<TMemberType>, SObject*>;

			template<class TMemberType>
			void SupportNativeObjectCollection(InheritSelector<1>&&) requires
				IDictionary<TMemberType, SObject*, DictionaryValue_t<TMemberType>>;

			template<class TMemberType>
			void SupportNativeObjectCollection(InheritSelector<1>&&) requires
				IGenericTuple<TMemberType>;

			template<class TMemberType>
			void SupportNativeObjectCollection(InheritSelector<0>&&);

		private:
			template<class TMemberType, size_t... Idx>
			void SupportNativeObjectCollectionTupleImpl(std::index_sequence<Idx...>&&);

			template<class T>
			SObject* MakeObjectPointerOrNull(T& value);

			template<class... TAttributeCollection, size_t... Idx>
			static std::vector<SFieldAttribute*> MakeVectorCollection(std::tuple<TAttributeCollection...>& attributes, std::index_sequence<Idx...>&&)
			{
				return std::vector<SFieldAttribute*>{ (&std::get<Idx>(attributes))... };
			}
		};
	}
}