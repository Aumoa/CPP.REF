// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FieldInfoMetadataGenerator.h"
#include "typeof.h"
#include "ReflectionTypeTraits.h"
#include <array>

namespace libty::inline Core::Reflection
{
	template<class TMemberType, class TOwningClass, class... TAttributeCollection>
	FieldInfoMetadataGenerator::FieldInfoMetadataGenerator(TMemberType TOwningClass::* field, std::string_view fieldName, std::tuple<TAttributeCollection...>& attributes)
		: FieldName(String::AsUnicode(fieldName))
		, bIsConst(false)
		, bIsStatic(false)
	{
		using TReflectType = RemoveObjectPointer_t<TMemberType>;

		Setter = [field](SObject* _This, SObject* _Value)
		{
			auto This = Cast<TOwningClass>(_This);
			auto Value = Cast<TReflectType>(_Value);
			if constexpr (std::is_pointer_v<decltype(This)>)
			{
				(This->*field) = Value;
			}
			else
			{
				(This.*field) = Value;
			}
		};

		Getter = [field](SObject* _This)
		{
			auto This = Cast<TOwningClass>(_This);
			if constexpr (std::is_pointer_v<decltype(This)>)
			{
				return Cast<SObject>(This->*field);
			}
			else
			{
				return Cast<SObject>(This.*field);
			}
		};

		GetterAny = [field](SObject* _This)
		{
			auto This = Cast<TOwningClass>(_This);
			if constexpr (std::is_pointer_v<decltype(This)>)
			{
				return This->*field;
			}
			else
			{
				return This.*field;
			}
		};

		FieldType = typeof(TReflectType);
		SupportNativeObjectCollection<TReflectType>(inh_select<2>());
		Attributes = MakeVectorCollection(attributes, std::make_index_sequence<sizeof...(TAttributeCollection)>{});
	}

	template<class TMemberType, class... TAttributeCollection>
	FieldInfoMetadataGenerator::FieldInfoMetadataGenerator(TMemberType* field, std::string_view fieldName, std::tuple<TAttributeCollection...>& attributes)
		: FieldName(String::AsUnicode(fieldName))
		, bIsConst(false)
		, bIsStatic(true)
	{
		using TReflectType = RemoveObjectPointer_t<TMemberType>;

		Setter = [field](SObject* _This, SObject* _Value)
		{
			auto Value = Cast<TReflectType>(_Value);
			*field = Value;
		};

		Getter = [field](SObject* _This)
		{
			return Cast<SObject>(*field);
		};

		GetterAny = [field](SObject* _This)
		{
			return *field;
		};

		FieldType = typeof(TReflectType);
		SupportNativeObjectCollection<TReflectType>(inh_select<2>());
		Attributes = MakeVectorCollection(attributes, std::make_index_sequence<sizeof...(TAttributeCollection)>{});
	}

	template<class TMemberType, class TOwningClass, class... TAttributeCollection>
	FieldInfoMetadataGenerator::FieldInfoMetadataGenerator(const TMemberType TOwningClass::* field, std::string_view fieldName, std::tuple<TAttributeCollection...>& attributes)
		: FieldName(String::AsUnicode(fieldName))
		, bIsConst(true)
		, bIsStatic(false)
	{
		using TReflectType = RemoveObjectPointer_t<TMemberType>;

		Setter = nullptr;

		Getter = [field](SObject* _This)
		{
			auto This = Cast<TOwningClass>(_This);
			return Cast<SObject>(This->*field);
		};

		GetterAny = [field](SObject* _This)
		{
			auto This = Cast<TOwningClass>(_This);
			return This->*field;
		};

		FieldType = typeof(TReflectType);
		SupportNativeObjectCollection<TReflectType>(inh_select<2>());
		Attributes = MakeVectorCollection(attributes, std::make_index_sequence<sizeof...(TAttributeCollection)>{});
	}

	template<class TMemberType, class... TAttributeCollection>
	FieldInfoMetadataGenerator::FieldInfoMetadataGenerator(const TMemberType* field, std::string_view fieldName, std::tuple<TAttributeCollection...>& attributes)
		: FieldName(String::AsUnicode(fieldName))
		, bIsConst(true)
		, bIsStatic(true)
	{
		using TReflectType = RemoveObjectPointer_t<TMemberType>;

		Setter = nullptr;

		Getter = [field](SObject* _This)
		{
			return Cast<SObject>(*field);
		};

		GetterAny = [field](SObject* _This)
		{
			return *field;
		};

		FieldType = typeof(TReflectType);
		SupportNativeObjectCollection<TReflectType>(inh_select<2>());
		Attributes = MakeVectorCollection(attributes, std::make_index_sequence<sizeof...(TAttributeCollection)>{});
	}

	template<class TMemberType>
	void FieldInfoMetadataGenerator::SupportNativeObjectCollection(InheritSelector<2>&&) requires
		IEnumerable<TMemberType, SObject*>
	{
		Collection = [](FieldInfoMetadataGenerator* _Caller, SObject* _This, NativeGCInvoke& _GCInvoke)
		{
			for (auto& item : std::any_cast<TMemberType>(_Caller->GetterAny(_This)))
			{
				_GCInvoke(item);
			}
		};
	}

	template<class TMemberType>
	void FieldInfoMetadataGenerator::SupportNativeObjectCollection(InheritSelector<2>&&) requires
		IDictionary<TMemberType, SObject*, SObject*>
	{
		Collection = [](FieldInfoMetadataGenerator* _Caller, SObject* _This, NativeGCInvoke& _GCInvoke)
		{
			for (auto& [key, value] : std::any_cast<TMemberType>(_Caller->GetterAny(_This)))
			{
				_GCInvoke(key);
				_GCInvoke(value);
			}
		};
	}

	template<class TMemberType>
	void FieldInfoMetadataGenerator::SupportNativeObjectCollection(InheritSelector<1>&&) requires
		IDictionary<TMemberType, DictionaryKey_t<TMemberType>, SObject*>
	{
		Collection = [](FieldInfoMetadataGenerator* _Caller, SObject* _This, NativeGCInvoke& _GCInvoke)
		{
			for (auto& [key, value] : std::any_cast<TMemberType>(_Caller->GetterAny(_This)))
			{
				_GCInvoke(value);
			}
		};
	}

	template<class TMemberType>
	void FieldInfoMetadataGenerator::SupportNativeObjectCollection(InheritSelector<1>&&) requires
		IDictionary<TMemberType, SObject*, DictionaryValue_t<TMemberType>>
	{
		Collection = [](FieldInfoMetadataGenerator* _Caller, SObject* _This, NativeGCInvoke& _GCInvoke)
		{
			for (auto& [key, value] : std::any_cast<TMemberType>(_Caller->GetterAny(_This)))
			{
				_GCInvoke(key);
			}
		};
	}

	template<class TMemberType>
	void FieldInfoMetadataGenerator::SupportNativeObjectCollection(InheritSelector<1>&&) requires
		IGenericTuple<TMemberType>
	{
		SupportNativeObjectCollectionTupleImpl(std::make_index_sequence<std::tuple_size_v<TMemberType>>{});
	}

	template<class TMemberType>
	void FieldInfoMetadataGenerator::SupportNativeObjectCollection(InheritSelector<0>&&)
	{
	}


	template<class TMemberType, size_t... Idx>
	void FieldInfoMetadataGenerator::SupportNativeObjectCollectionTupleImpl(std::index_sequence<Idx...>&&)
	{
		Collection = [](FieldInfoMetadataGenerator* _Caller, SObject* _This, NativeGCInvoke& _GCInvoke)
		{
			auto tuple = std::any_cast<TMemberType>(_Caller->GetterAny(_This));
			for (auto& obj : { _Caller->MakeObjectPointerOrNull(std::get<Idx>(tuple))... })
			{
				_GCInvoke(obj);
			}
		};
	}

	template<class T>
	SObject* FieldInfoMetadataGenerator::MakeObjectPointerOrNull(T& value)
	{
		if constexpr (std::derived_from<T, SObject>)
		{
			return value;
		}
		else
		{
			return nullptr;
		}
	}
}