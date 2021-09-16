// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <typeinfo>
#include <functional>
#include "PrimitiveTypes.h"
#include "Concepts/CoreConcepts.h"
#include "Method.h"
#include "Property.h"
#include "Misc/StringUtils.h"

class SObject;
class Method;
class Property;

class CORE_API Type
{
	using ObjectCtor = SObject*(*)();
	Type() = delete;
	Type(const Type&) = delete;

	size_t _typeHash = 0;
	std::wstring _friendlyName;
	Type* _superClass = nullptr;
	ObjectCtor _ctor;
	std::vector<Method> _functions;
	std::vector<Property> _properties;
	uint8 _bNative : 1 = false;

public:
	template<class TType>
	struct TypeGenerator
	{
		std::wstring FriendlyName;
		std::vector<Method> Functions;
		std::vector<Property> Properties;
		Type* SuperClass = nullptr;
		bool bNative = false;

		TypeGenerator(std::wstring_view friendlyName) requires std::derived_from<TType, SObject>
			: FriendlyName(friendlyName)
			, bNative(false)
		{
			Functions.reserve(100);
			CollectFunctions<0>(Functions);
			CollectProperties<0>(Properties);

			if constexpr (HasSuper<TType>)
			{
				using SuperType = typename TType::Super;
				SuperClass = SuperType::StaticClass();
			}
		}

		TypeGenerator()
			: FriendlyName(ANSI_TO_WCHAR(typeid(TType).name()))
			, bNative(true)
		{
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

		template<size_t N>
		static void CollectProperties(std::vector<Property>& properties)
		{
			if constexpr (std::same_as<decltype(TType::template REFLECTION_GetPropertyPointer<N>(0)), void>)
			{
				return;
			}
			else
			{
				properties.emplace_back(TType::template REFLECTION_GetPropertyPointer<N>(0));
				CollectProperties<N + 1>(properties);
			}
		}
	};

public:
	template<class TType>
	Type(TypeGenerator<TType>&& generator)
		: _typeHash(typeid(TType).hash_code())
		, _friendlyName(std::move(generator.FriendlyName))
		, _superClass(generator.SuperClass)
		, _ctor(GetConstructorFunctionBody<TType>((int32)0))
		, _functions(std::move(generator.Functions))
		, _properties(std::move(generator.Properties))
		, _bNative(generator.bNative)
	{
	}

	const std::wstring& GetFriendlyName() const;
	size_t GetHashCode() const;

	Type* GetSuper() const;
	SObject* Instantiate() const;
	inline bool IsNativeType() const { return _bNative; }

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
	std::vector<Property> GetProperties(bool bIncludeSuperMembers = true) const;
	const Property* GetProperty(std::wstring_view friendlyName, bool bIncludeSuperMembers = true) const;

	template<class T> requires requires { T::StaticClass(); }
	static auto GetStaticClass()
	{
		return T::StaticClass();
	}

	template<class T> requires (!requires { T::StaticClass(); })
	static auto GetStaticClass()
	{
		static auto MyClassType = Type(TypeGenerator<T>());
		return &MyClassType;
	}

private:
	template<std::derived_from<SObject> TType> requires std::constructible_from<TType>
	static ObjectCtor GetConstructorFunctionBody(int32)
	{
		return +[]() -> SObject*
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