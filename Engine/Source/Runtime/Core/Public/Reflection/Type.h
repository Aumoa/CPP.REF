// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <typeinfo>
#include <functional>
#include "PrimitiveTypes.h"
#include "Concepts/CoreConcepts.h"
#include "Method.h"

class SObject;
class Method;

class CORE_API Type
{
	using ObjectCtor = std::function<SObject*()>;
	Type() = delete;
	Type(const Type&) = delete;

	size_t _typeHash = 0;
	std::wstring _friendlyName;
	Type* _superClass = nullptr;
	ObjectCtor _ctor;
	std::vector<Method> _functions;

public:
	template<class TType>
	struct TypeGenerator
	{
		std::wstring_view FriendlyName;
		std::vector<Method> Functions;
		Type* SuperClass = nullptr;

		TypeGenerator(std::wstring_view friendlyName)
			: FriendlyName(friendlyName)
		{
			Functions.reserve(100);
			CollectFunctions<0>(Functions);

			if constexpr (HasSuper<TType>)
			{
				using SuperType = typename TType::Super;
				SuperClass = SuperType::StaticClass();
			}
		}

	private:
		template<size_t N>
		static void CollectFunctions(std::vector<Method>& methods)
		{
			if constexpr (std::same_as<decltype(TType::template REFLECTION_GetFunctionPointer<N>(0)), void>)
			{
				return;
			}
			else
			{
				methods.emplace_back(TType::template REFLECTION_GetFunctionPointer<N>(0), std::wstring_view(TType::template REFLECTION_GetFunctionName<N>(0)));
				CollectFunctions<N + 1>(methods);
			}
		}
	};

public:
	template<class TType>
	Type(TypeGenerator<TType>&& generator)
		: _typeHash(typeid(TType).hash_code())
		, _friendlyName(generator.FriendlyName)
		, _superClass(generator.SuperClass)
		, _ctor(GetConstructorFunctionBody<TType>((int32)0))
		, _functions(std::move(generator.Functions))
	{
	}

	const std::wstring& GetFriendlyName() const;
	size_t GetHashCode() const;

	Type* GetSuper() const;
	SObject* Instantiate() const;
	bool IsDerivedFrom(const Type* type) const;
	bool IsBaseOf(const Type* type) const;
	bool IsA(const Type* type) const;

	template<std::derived_from<SObject> T>
	bool IsDerivedFrom() const { return IsDerivedFrom(T::StaticClass()); }
	template<std::derived_from<SObject> T>
	bool IsBaseOf() const { return IsBaseOf(T::StaticClass()); }
	template<std::derived_from<SObject> T>
	bool IsA() const { return IsA(T::StaticClass()); }

	std::vector<Method> GetMethods(bool bIncludeSuperMembers = true) const;
	const Method* GetMethod(std::wstring_view friendlyName, bool bIncludeSuperMembers = true) const;

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