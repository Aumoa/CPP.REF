// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include <vector>
#include <functional>
#include <span>

namespace libty::inline Core
{
	class SObject;
	class SType;

	inline namespace Attributes
	{
		class SMethodAttribute;
	}

	namespace Reflection
	{
		struct MethodInfoMetadataGenerator
		{
			using MethodAttributeCollection = std::vector<SMethodAttribute*>;
			using ParameterTypeCollection = std::vector<SType*>;
			using Invoke = std::function<SObject*(SObject* _This, std::span<SObject* const> _Args)>;

			std::wstring MethodName;
			MethodAttributeCollection Attributes;

			uint8 bIsStatic : 1;

			SType* OwningType;
			SType* ReturnType;
			ParameterTypeCollection ParameterTypes;
			Invoke Invoker;

			template<class TOwningClass, class TReturnType, class... TArgs, class... TAttributeCollection>
			static MethodInfoMetadataGenerator Generate
			(
				TReturnType(TOwningClass::* method)(TArgs...),
				std::string_view methodName,
				std::tuple<TAttributeCollection...>& attributes
			)
			{
				return Generate<TOwningClass>(method, methodName, attributes, std::make_index_sequence<sizeof...(TArgs)>{});
			}

			template<class TOwningClass, class TReturnType, class... TArgs, class... TAttributeCollection, size_t... Idx>
			static MethodInfoMetadataGenerator Generate
			(
				TReturnType(TOwningClass::* method)(TArgs...),
				std::string_view methodName,
				std::tuple<TAttributeCollection...>& attributes,
				std::index_sequence<Idx...>&&
			);

			template<class TOwningClass, class TReturnType, class... TArgs, class... TAttributeCollection>
			static MethodInfoMetadataGenerator Generate
			(
				TReturnType(*method)(TArgs...),
				std::string_view methodName,
				std::tuple<TAttributeCollection...>& attributes
			)
			{
				return Generate<TOwningClass>(method, methodName, attributes, std::make_index_sequence<sizeof...(TArgs)>{});
			}

			template<class TOwningClass, class TReturnType, class... TArgs, class... TAttributeCollection, size_t... Idx>
			static MethodInfoMetadataGenerator Generate
			(
				TReturnType(*method)(TArgs...),
				std::string_view methodName,
				std::tuple<TAttributeCollection...>& attributes,
				std::index_sequence<Idx...>&&
			);

		private:
			template<class... TAttributeCollection, size_t... Idx>
			static std::vector<SMethodAttribute*> MakeAttributeCollection(std::tuple<TAttributeCollection...>& attributes, std::index_sequence<Idx...>&&)
			{
				return std::vector<SMethodAttribute*>{ (&std::get<Idx>(attributes))... };
			}
		};
	}
}