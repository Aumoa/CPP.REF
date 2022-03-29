// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "MethodInfoMetadataGenerator.h"
#include "typeof.h"
#include "ReflectionTypeTraits.h"
#include "ValueType.h"
#include "Misc/String.h"

namespace libty::Core::Reflection
{
	template<class TOwningClass, class TReturnType, class... TArgs, class... TAttributeCollection, size_t... Idx>
	MethodInfoMetadataGenerator MethodInfoMetadataGenerator::Generate
	(
		TReturnType(TOwningClass::* method)(TArgs...),
		std::string_view methodName,
		std::tuple<TAttributeCollection...>& attributes,
		std::index_sequence<Idx...>&&
	)
	{
		MethodInfoMetadataGenerator gen;
		gen.MethodName = String::AsUnicode(methodName);
		gen.Attributes = MakeAttributeCollection(attributes, std::make_index_sequence<sizeof...(TAttributeCollection)>{});
		gen.bIsStatic = false;
		gen.OwningType = typeof(TOwningClass);
		gen.ReturnType = typeof(RemoveObjectPointer_t<TReturnType>);
		gen.ParameterTypes = ParameterTypeCollection{ typeof(RemoveObjectPointer_t<TArgs>)... };
		gen.Invoker = [method](SObject* _This, std::span<SObject* const> _Args) -> SObject*
		{
			if constexpr (std::same_as<TReturnType, void>)
			{
				(Cast<TOwningClass>(_This)->*method)(Cast<RemoveObjectPointer_t<TArgs>>(_Args[Idx])...);
				return nullptr;
			}
			else
			{
				TReturnType Return = (Cast<TOwningClass>(_This)->*method)(Cast<RemoveObjectPointer_t<TArgs>>(_Args[Idx])...);
				return Cast<SObject>(Return);
			}
		};

		return gen;
	}

	template<class TOwningClass, class TReturnType, class... TArgs, class... TAttributeCollection, size_t... Idx>
	MethodInfoMetadataGenerator MethodInfoMetadataGenerator::Generate
	(
		TReturnType(* method)(TArgs...),
		std::string_view methodName,
		std::tuple<TAttributeCollection...>& attributes,
		std::index_sequence<Idx...>&&
	)
	{
		MethodInfoMetadataGenerator gen;
		gen.MethodName = String::AsUnicode(methodName);
		gen.Attributes = MakeAttributeCollection(attributes, std::make_index_sequence<sizeof...(TAttributeCollection)>{});
		gen.bIsStatic = true;
		gen.OwningType = typeof(TOwningClass);
		gen.ReturnType = typeof(RemoveObjectPointer_t<TReturnType>);
		gen.ParameterTypes = ParameterTypeCollection{ typeof(RemoveObjectPointer_t<TArgs>)... };
		gen.Invoker = [method](SObject* _This, std::span<SObject* const> _Args) -> SObject*
		{
			if constexpr (std::same_as<TReturnType, void>)
			{
				(*method)(Cast<RemoveObjectPointer_t<TArgs>>(_Args[Idx])...);
				return nullptr;
			}
			else
			{
				TReturnType Return = (*method)(Cast<RemoveObjectPointer_t<TArgs>>(_Args[Idx])...);
				return Cast<SObject>(Return);
			}
		};

		return gen;
	}
}