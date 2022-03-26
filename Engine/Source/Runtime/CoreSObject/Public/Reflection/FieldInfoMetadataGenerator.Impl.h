// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "FieldInfoMetadataGenerator.h"
#include "typeof.h"
#include <array>

namespace libty::Core::Reflection
{
	template<class TMemberType, class TOwningClass>
	FieldInfoMetadataGenerator::FieldInfoMetadataGenerator(TMemberType TOwningClass::* field, std::string_view fieldName, FieldAttributeCollection attributes)
		: FieldName(String::AsUnicode(fieldName))
		, Attributes(std::move(attributes))
		, bIsConst(false)
		, bIsStatic(false)
	{

		Setter = [field](SObject* _This, SObject* _Value)
		{
			auto This = Cast<TOwningClass>(_This);
			auto Value = Cast<TMemberType>(_Value);
			(This->*field) = Value;
		};

		Getter = [field](SObject* _This)
		{
			auto This = Cast<TOwningClass>(_This);
			return Cast<SObject>(This->*field);
		};

		FieldType = typeof(TMemberType);
		SupportNativeObjectCollection<TMemberType>(inh_select<2>());
	}
		
	template<class TMemberType>
	FieldInfoMetadataGenerator::FieldInfoMetadataGenerator(TMemberType* field, std::string_view fieldName, FieldAttributeCollection attributes)
		: FieldName(String::AsUnicode(fieldName))
		, Attributes(std::move(attributes))
		, bIsConst(false)
		, bIsStatic(true)
	{
		Setter = [field](SObject* _This, SObject* _Value)
		{
			auto Value = Cast<TMemberType>(_Value);
			*field = Value;
		};

		Getter = [field](SObject* _This)
		{
			return Cast<SObject>(*field);
		};

		FieldType = typeof(TMemberType);
		SupportNativeObjectCollection<TMemberType>(inh_select<2>());
	}
		
	template<class TMemberType, class TOwningClass>
	FieldInfoMetadataGenerator::FieldInfoMetadataGenerator(const TMemberType TOwningClass::* field, std::string_view fieldName, FieldAttributeCollection attributes)
		: FieldName(String::AsUnicode(fieldName))
		, Attributes(std::move(attributes))
		, bIsConst(true)
		, bIsStatic(false)
	{
		Setter = nullptr;

		Getter = [field](SObject* _This)
		{
			auto This = Cast<TOwningClass>(_This);
			return Cast<SObject>(This->*field);
		};

		FieldType = typeof(TMemberType);
		SupportNativeObjectCollection<TMemberType>(inh_select<2>());
	}

	template<class TMemberType>
	FieldInfoMetadataGenerator::FieldInfoMetadataGenerator(const TMemberType* field, std::string_view fieldName, FieldAttributeCollection attributes)
		: FieldName(String::AsUnicode(fieldName))
		, Attributes(std::move(attributes))
		, bIsConst(true)
		, bIsStatic(true)
	{
		Setter = nullptr;

		Getter = [field](SObject* _This)
		{
			return Cast<SObject>(*field);
		};

		FieldType = typeof(TMemberType);
		SupportNativeObjectCollection<TMemberType>(inh_select<2>());
	}

	template<class TMemberType>
	void FieldInfoMetadataGenerator::SupportNativeObjectCollection(InheritSelector<2>&&) requires
		IEnumerable<TMemberType, SObject*>
	{
		Collection = [this](SObject* _This, NativeGCInvoke& _GCInvoke)
		{
			for (auto& item : Cast<TMemberType>(Getter(_This)))
			{
				_GCInvoke(item);
			}
		};
	}

	template<class TMemberType>
	void FieldInfoMetadataGenerator::SupportNativeObjectCollection(InheritSelector<2>&&) requires
		IDictionary<TMemberType, SObject*, SObject*>
	{
		Collection = [this](SObject* _This, NativeGCInvoke& _GCInvoke)
		{
			for (auto& [key, value] : Cast<TMemberType>(Getter(_This)))
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
		Collection = [this](SObject* _This, NativeGCInvoke& _GCInvoke)
		{
			for (auto& [key, value] : Cast<TMemberType>(Getter(_This)))
			{
				_GCInvoke(value);
			}
		};
	}

	template<class TMemberType>
	void FieldInfoMetadataGenerator::SupportNativeObjectCollection(InheritSelector<1>&&) requires
		IDictionary<TMemberType, SObject*, DictionaryValue_t<TMemberType>>
	{
		Collection = [this](SObject* _This, NativeGCInvoke& _GCInvoke)
		{
			for (auto& [key, value] : Cast<TMemberType>(Getter(_This)))
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
		Collection = [this](SObject* _This, NativeGCInvoke& _GCInvoke)
		{
			auto tuple = Cast<TMemberType>(Getter(_This));
			for (auto& obj : { MakeObjectPointerOrNull(std::get<Idx>(tuple))... })
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