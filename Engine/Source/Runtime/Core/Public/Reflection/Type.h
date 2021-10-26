// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <typeinfo>
#include <functional>
#include <unordered_map>
#include <mutex>
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
	using ObjectCtor = SObject*(*)(SObject* InOuter);
	Type() = delete;
	Type(const Type&) = delete;

private:
	static std::unordered_map<size_t, uint64> TypeRegister;
	static std::mutex TypeRegisterMutex;

	size_t TypeHash = 0;
	std::wstring FriendlyName;
	Type* SuperClass = nullptr;
	ObjectCtor Constructor;
	std::vector<Method> Functions;
	std::vector<Property> Properties;
	uint8 bNative : 1 = false;

public:
	template<class TType>
	struct TypeGenerator
	{
		std::wstring FriendlyName;
		std::vector<Method> Functions;
		std::vector<Property> Properties;
		Type* SuperClass = nullptr;
		bool bNative = false;

		TypeGenerator(std::wstring_view InFriendlyName) requires requires { TType::StaticClass(); }
			: FriendlyName(InFriendlyName)
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
		static void CollectFunctions(std::vector<Method>& Methods)
		{
			if constexpr (std::same_as<decltype(TType::template REFLECTION_GetFunctionPointer<N>(0)), void>)
			{
				return;
			}
			else
			{
				Methods.emplace_back(TType::template REFLECTION_GetFunctionPointer<N>(0), std::wstring_view(TType::template REFLECTION_GetFunctionName<N>(0)));
				CollectFunctions<N + 1>(Methods);
			}
		}

		template<size_t N>
		static void CollectProperties(std::vector<Property>& Properties)
		{
			if constexpr (std::same_as<decltype(TType::template REFLECTION_GetPropertyPointer<N>(0)), void>)
			{
				return;
			}
			else
			{
				Properties.emplace_back(TType::template REFLECTION_GetPropertyPointer<N>(0));
				CollectProperties<N + 1>(Properties);
			}
		}
	};

public:
	template<class TType>
	Type(TypeGenerator<TType>&& Generator)
		: TypeHash(typeid(TType).hash_code())
		, FriendlyName(std::move(Generator.FriendlyName))
		, SuperClass(Generator.SuperClass)
		, Constructor(GetConstructorFunctionBody<TType>((int32)0))
		, Functions(std::move(Generator.Functions))
		, Properties(std::move(Generator.Properties))
		, bNative(Generator.bNative)
	{
	}

	const std::wstring& GetFriendlyName() const;
	size_t GetHashCode() const;

	Type* GetSuper() const;
	SObject* Instantiate(SObject* InOuter) const;
	inline bool IsNativeType() const { return bNative; }

	bool IsDerivedFrom(const Type* InType) const;
	bool IsBaseOf(const Type* InType) const;
	bool IsA(const Type* InType) const;

	template<std::derived_from<SObject> T>
	bool IsDerivedFrom() const { return IsDerivedFrom(T::StaticClass()); }
	template<std::derived_from<SObject> T>
	bool IsBaseOf() const { return IsBaseOf(T::StaticClass()); }
	template<std::derived_from<SObject> T>
	bool IsA() const { return IsA(T::StaticClass()); }

	std::vector<Method> GetMethods(bool bIncludeSuperMembers = true) const;
	const Method* GetMethod(std::wstring_view InFriendlyName, bool bIncludeSuperMembers = true) const;
	std::vector<Property> GetProperties(bool bIncludeSuperMembers = true) const;
	const Property* GetProperty(std::wstring_view InFriendlyName, bool bIncludeSuperMembers = true) const;

	std::wstring GenerateUniqueName();

public:
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
		return +[](SObject* InOuter) -> SObject*
		{
			return InOuter->template NewObject<TType>();
		};
	}

	template<class TType>
	static ObjectCtor GetConstructorFunctionBody(float)
	{
		return nullptr;
	}
};