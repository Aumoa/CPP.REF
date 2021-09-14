// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <typeinfo>
#include <functional>
#include "PrimitiveTypes.h"
#include "Concepts/CoreConcepts.h"

class SObject;

class CORE_API Type
{
	using ObjectCtor = std::function<SObject*()>;

	std::wstring _friendlyName;
	ObjectCtor _ctor;

public:
	std::vector<std::wstring> _functions;

public:
	template<class TType>
	struct TypeGenerator
	{
		std::wstring_view FriendlyName;
		std::vector<std::wstring> Functions;

		TypeGenerator(std::wstring_view friendlyName)
			: FriendlyName(friendlyName)
			, Functions(AsVector(CollectFunctions<TType, 0>()))
		{
		}

	private:
		template<size_t N>
		static std::vector<std::wstring> AsVector(const std::array<const wchar_t*, N>& arr)
		{
			std::vector<std::wstring> vec;
			vec.reserve(N);
			for (size_t i = 0; i < N; ++i)
			{
				vec.emplace_back(arr[i]);
			}
			return vec;
		}

		template<class T, size_t N>
		static consteval auto CollectFunctions()
		{
			if constexpr (std::same_as<decltype(T::template REFLECTION_GetFunctionName<N>(0)), void>)
			{
				if constexpr (HasSuper<T>)
				{
					return CollectFunctions<typename T::Super, 0>();
				}
				else
				{
					return std::array<const wchar_t*, 0>{};
				}
			}
			else
			{
				constexpr const wchar_t* functionName = T::template REFLECTION_GetFunctionName<N>(0);
				std::array functions = CollectFunctions<T, N + 1>();
				return insert_front(functionName, functions);
			}
		}

		template<class T, size_t N>
		static consteval auto insert_front(T v, std::array<T, N> c)
		{
			return insert_front(v, c, std::make_index_sequence<N>{});
		}

		template<class T, size_t... N>
		static consteval auto insert_front(T v, std::array<T, sizeof...(N)> c, std::index_sequence<N...>&&)
		{
			return std::array<T, sizeof...(N) + 1>{ v, c[N]... };
		}
	};

public:
	template<class TType>
	Type(TypeGenerator<TType>&& generator)
		: _friendlyName(generator.FriendlyName)
		, _ctor(GetConstructorFunctionBody<TType>((int32)0))
		, _functions(std::move(generator.Functions))
	{
	}

	const std::wstring& GetFriendlyName() const;
	SObject* Instantiate() const;

private:
	template<class TType> requires std::constructible_from<TType>
	static ObjectCtor GetConstructorFunctionBody(int32)
	{
		return []() -> SObject*
		{
			return new TType();
		};
	}

	template<class TType>
	static ObjectCtor GetConstructorFunctionBody(float)
	{
		return nullptr;
	}
};