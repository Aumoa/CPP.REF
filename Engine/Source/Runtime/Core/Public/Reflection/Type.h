// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <typeinfo>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <map>
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
	std::wstring FullName;
	Type* SuperClass = nullptr;
	ObjectCtor Constructor;
	std::vector<Method> Functions;
	std::vector<Property> Properties;
	uint8 bNative : 1 = false;
	uint8 bPropertyCached : 1 = false;
	uint8 bFunctionCached : 1 = false;

	std::function<SObject* (void*)> ToObjectCast;
	std::function<void* (SObject*)> FromObjectCast;

public:
	template<class TType>
	struct TypeGenerator
	{
		std::wstring FriendlyName;
		std::wstring FullName;
		std::vector<Method> Functions;
		std::vector<Property> Properties;
		Type* SuperClass = nullptr;
		bool bNative = false;
		std::function<SObject* (void*)> ToObjectCast;
		std::function<void* (SObject*)> FromObjectCast;

		TypeGenerator(std::wstring_view InFriendlyName, std::wstring_view FullName) requires requires { TType::StaticClass(); }
			: FriendlyName(InFriendlyName)
			, FullName(FullName)
			, bNative(false)
			, ToObjectCast(+[](void* UnknownType) { return static_cast<SObject*>(reinterpret_cast<TType*>(UnknownType)); })
			, FromObjectCast(+[](SObject* Object) { return reinterpret_cast<void*>(dynamic_cast<TType*>(Object)); })
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

private:
	void RegisterStaticClass();

public:
	template<class TType>
	Type(TypeGenerator<TType>&& Generator)
		: TypeHash(typeid(TType).hash_code())
		, FriendlyName(std::move(Generator.FriendlyName))
		, FullName(std::move(Generator.FullName))
		, SuperClass(Generator.SuperClass)
		, Constructor(GetConstructorFunctionBody<TType>((int32)0))
		, Functions(std::move(Generator.Functions))
		, Properties(std::move(Generator.Properties))
		, bNative(Generator.bNative)
		, ToObjectCast(std::move(Generator.ToObjectCast))
		, FromObjectCast(std::move(Generator.FromObjectCast))
	{
		RegisterStaticClass();
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

	template<class T> requires (!std::derived_from<T, SObject>)
	bool IsDerivedFrom() const { return IsDerivedFrom(GetStaticClass<T>()); }
	template<class T> requires (!std::derived_from<T, SObject>)
	bool IsBaseOf() const { return IsBaseOf(GetStaticClass<T>()); }
	template<class T> requires (!std::derived_from<T, SObject>)
	bool IsA() const { return IsA(GetStaticClass<T>()); }

private:
	std::vector<Property*> CachedProperties;
	std::vector<Property*> CachedFullProperties;
	std::vector<Property*> CachedGCProperties;

private:
	void CacheProperties();

public:
	std::vector<Method> GetMethods(bool bIncludeSuperMembers = true);
	Method* GetMethod(std::wstring_view InFriendlyName, bool bIncludeSuperMembers = true);
	const std::vector<Property*>& GetProperties(bool bIncludeSuperMembers = true);
	const std::vector<Property*>& GetGCProperties();
	Property* GetProperty(std::wstring_view InFriendlyName, bool bIncludeSuperMembers = true);

public:
	void* FromObject(SObject* Object) const { return FromObjectCast(Object); }
	SObject* ToObject(void* UnknownType) const { return ToObjectCast(UnknownType); }

	std::wstring GenerateUniqueName();

public:
	template<class T> requires requires { T::StaticClass(); }
	static auto GetStaticClass()
	{
		return T::StaticClass();
	}

	template<class T> requires
		std::is_pointer_v<T> &&
		requires { std::remove_pointer_t<T>::StaticClass(); }
	static auto GetStaticClass()
	{
		return std::remove_pointer_t<T>::StaticClass();
	}

	template<class T> requires
		(!requires { T::StaticClass(); }) &&
		(!std::same_as<T, void>) &&
		(!requires { std::remove_pointer_t<T>::StaticClass(); })
	static auto GetStaticClass()
	{
		static auto MyClassType = Type(TypeGenerator<T>());
		return &MyClassType;
	}

	template<std::same_as<void> T>
	static Type* GetStaticClass()
	{
		return nullptr;
	}

	static Type* FindStaticClass(std::wstring_view InFriendlyName);
	static std::span<Type*> FindAllSubclass(Type* BaseClass);
	template<class T>
	static std::span<Type*> FindAllSubclass() { return FindAllSubclass(GetStaticClass<T>()); }

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

// Declared in Method.h
template<bool bRet, class T, class... TArgs>
void Method::ProcessParameters()
{
	Type* Myt = Type::GetStaticClass<T>();

	if constexpr (bRet)
	{
		ReturnT = Myt;
	}
	else
	{
		Parameters.emplace_back(Type::GetStaticClass<T>());
	}

	if constexpr (sizeof...(TArgs) != 0)
	{
		ProcessParameters<false, TArgs...>();
	}
}