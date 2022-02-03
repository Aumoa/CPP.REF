// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <typeinfo>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <map>
#include "PrimitiveTypes.h"
#include "CoreConcepts.h"
#include "Method.h"
#include "Property.h"
#include "ObjectBase.h"
#include "GC/GC.h"
#include "Misc/StringUtils.h"
#include "Reflection/ReflectionMacros.h"

class SObject;
class Method;
class Property;

namespace ReflectionMacros
{
	template<class T>
	struct SuperClassTypeDeclare
	{
	private:
		template<class U> requires requires { typename U::This; }
		static auto Impl(int32) -> decltype(std::declval<typename U::This>());

		template<class U> requires (!requires { typename U::This; })
		static auto Impl(int16) -> void;

	public:
		using Type = std::remove_reference_t<decltype(Impl<T>(0))>;
	};

	template<class T>
	consteval bool IsStaticMember();
	template<class T>
	consteval bool IsStaticMember(T*) { return true; }
	template<class TOwner, class T>
	consteval bool IsStaticMember(T(TOwner::*)) { return false; }
	template<class T>
	consteval bool IsStaticMember(const T*) { return true; }
	template<class TOwner, class T>
	consteval bool IsStaticMember(const T(TOwner::*)) { return false; }

	template<class T>
	inline T& Assign(T& Member, const void* Value)
	{
		Member = *reinterpret_cast<const T*>(Value);
		return Member;
	}
}

class CORESOBJECT_API Type
{
	using ObjectCtor = SObject*(*)();
	Type() = delete;
	Type(const Type&) = delete;

private:
	static std::unordered_map<size_t, uint64> TypeRegister;
	static std::mutex TypeRegisterMutex;

	size_t TypeHash = 0;
	size_t SizeOf = 0;
	std::wstring FriendlyName;
	std::wstring FullName;
	Type* SuperClass = nullptr;
	ObjectCtor Constructor;
	std::vector<Method> Functions;
	std::vector<Property> Properties;
	std::function<SObject* (void*)> ToObjectCast;
	std::function<void* (SObject*)> FromObjectCast;

	uint8 bNative : 1 = false;
	uint8 bPropertyCached : 1 = false;
	uint8 bFunctionCached : 1 = false;

	uint8 bGCCollection : 1 = false;
	std::function<int32(void*, int32)> Collector;

public:
	template<class TType>
	struct TypeGenerator
	{
	private:
		template<class U>
		static bool CollectSingleNode(int32& Counter, U*& Node, int32& Depth) requires
			std::derived_from<U, SObject> &&
			requires { Node = nullptr; }
		{
			if (Node)
			{
				if (GC.PendingFinalize.contains(Node))
				{
					Node = nullptr;
				}
				else
				{
					GC.GCMarkingBuffer[Node->InternalIndex] = Depth;
					++Counter;
				}
			}

			return true;
		}

		template<class U>
		static bool CollectSingleNode(int32& Counter, U* const& Node, int32& Depth) requires
			std::derived_from<U, SObject> &&
			(!requires { Node = nullptr; })
		{
			if (Node)
			{
				if (GC.PendingFinalize.contains(Node))
				{
					return false;
				}
				else
				{
					GC.GCMarkingBuffer[Node->InternalIndex] = Depth;
					++Counter;
				}
			}

			return true;
		}

		template<class U>
		static bool CollectSingleNode(int32& Counter, U& Node, int32& Depth) requires
			requires { U::StaticClass(); }
		{
			Type* StructType = Node.GetType();
			Counter += StructType->Collector(&Node, Depth);
			return true;
		}

		template<class U>
		static bool CollectSingleNode(int32& Counter, const U& Node, int32& Depth)
		{
			return true;
		}

		template<class U1, class U2>
		static bool CollectSingleNode(int32& Counter, std::pair<U1, U2>& Node, int32& Depth)
		{
			bool b1 = CollectSingleNode(Counter, Node.first, Depth);
			bool b2 = CollectSingleNode(Counter, Node.second, Depth);
			return b1 || b2;
		}

		template<class... U>
		static bool CollectSingleNode(int32& Counter, std::tuple<U...>& Node, int32& Depth)
		{
			return CollectSingleNode(Counter, Node, Depth, std::make_index_sequence<sizeof...(U)>{});
		}

		template<class... U, size_t... _Idx>
		static bool CollectSingleNode(int32& Counter, std::tuple<U...>& Node, int32& Depth, std::index_sequence<_Idx...>&&)
		{
			std::array<bool, sizeof...(U)> Booleans = { CollectSingleNode(Counter, std::get<_Idx>(Node), Depth)... };
			for (auto& Bool : Booleans)
			{
				if (Bool)
				{
					return true;
				}
			}
			return false;
		}

		template<class U>
		static bool CollectSingleNode(int32& Counter, U& Node, int32& Depth) requires requires
		{
			{ std::begin(Node) };
			{ std::end(Node) };
		}
		{
			Type* CollectionType = Type::GetStaticClass<U>();
			Counter += CollectionType->Collector(&Node, Depth);
			return true;
		}

	public:
		std::wstring FriendlyName;
		std::wstring FullName;
		std::vector<Method> Functions;
		std::vector<Property> Properties;
		Type* SuperClass = nullptr;
		std::function<SObject* (void*)> ToObjectCast;
		std::function<void* (SObject*)> FromObjectCast;
		uint8 bNative : 1 = false;

		uint8 bGCCollection : 1 = false;
		std::function<int32(void*, int32)> Collector;

		// SObject type.
		TypeGenerator(std::wstring_view InFriendlyName, std::wstring_view FullName) requires
			requires { TType::StaticClass(); }
			: FriendlyName(InFriendlyName)
			, FullName(FullName)
			, bNative(false)
			, ToObjectCast(+[](void* UnknownType) { return dynamic_cast<SObject*>(reinterpret_cast<TType*>(UnknownType)); })
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

		// Struct type.
		TypeGenerator(std::wstring_view InFriendlyName) requires
			requires { TType::StaticClass(); }
			: FriendlyName(InFriendlyName)
			, FullName(ANSI_TO_WCHAR(typeid(TType).name()))
			, bNative(true)
			, bGCCollection(true)
		{
			Functions.reserve(100);
			CollectFunctions<0>(Functions);
			CollectProperties<0>(Properties);

			if constexpr (HasSuper<TType>)
			{
				using SuperType = typename TType::Super;
				SuperClass = SuperType::StaticClass();
			}

			Collector = [](void* Ptr, int32 Depth) -> int32
			{
				TType& Struct = *reinterpret_cast<TType*>(Ptr);
				Type* StructType = Struct.GetType();

				int32 Count = 0;
				for (auto& Prop : StructType->GetGCProperties())
				{
					Type* PropertyType = Prop->GetMemberType();
					if (PropertyType->IsGCCollection())
					{
						// Mark collection objects.
						uint8& CollectionPtr = const_cast<uint8&>(Prop->template GetValue<uint8>(Struct));
						Count += PropertyType->Collector(&CollectionPtr, Depth);
					}
					else
					{
						SObject* Object = Prop->GetObject(Struct);
						if (Object)
						{
							if (GC.PendingFinalize.contains(Object))
							{
								Prop->SetObject(Struct, nullptr);
							}
							else
							{
								GC.GCMarkingBuffer[Object->InternalIndex] = Depth;
								++Count;
							}
						}
					}
				}

				return Count;
			};
		}

		// SObject container type.
		TypeGenerator() requires requires(const TType& Collection)
			{
				{ std::begin(Collection) };
				{ std::end(Collection) };
			}
			: FriendlyName(ANSI_TO_WCHAR(typeid(TType).name()))
			, FullName(FriendlyName)
			, bNative(true)
			, bGCCollection(true)
			, Collector([](void* Ptr, int32 Depth) -> int32
			{
				TType& Collection = *reinterpret_cast<TType*>(Ptr);
				int32 Count = 0;
				for (auto It = Collection.begin(); It != Collection.end();)
				{
					auto& Object = *It;
					if (!CollectSingleNode(Count, Object, Depth))
					{
						if constexpr (!IsMutableCollection<TType>)
						{
							Collection.erase(It++);
							continue;
						}
					}
					++It;
				}
				return Count;
			})
		{
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
		, SizeOf(sizeof(TType))
		, FriendlyName(std::move(Generator.FriendlyName))
		, FullName(std::move(Generator.FullName))
		, SuperClass(Generator.SuperClass)
		, Constructor(GetConstructorFunctionBody<TType>((int32)0))
		, Functions(std::move(Generator.Functions))
		, Properties(std::move(Generator.Properties))
		, ToObjectCast(std::move(Generator.ToObjectCast))
		, FromObjectCast(std::move(Generator.FromObjectCast))
		, bNative(Generator.bNative)
		, bGCCollection(Generator.bGCCollection)
		, Collector(std::move(Generator.Collector))
	{
		RegisterStaticClass();
	}

	const std::wstring& GetFriendlyName() const;
	const std::wstring& GetFullName() const;
	size_t GetHashCode() const;
	size_t GetSizeOf() const;

	Type* GetSuper() const;
	SObject* Instantiate() const;
	inline bool IsNativeType() const { return bNative; }
	inline bool IsGCCollection() const { return bGCCollection; }

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
	int32 MarkCollectionObjects(SObject* Object, Property* CollectionProp, int32 Depth);

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

	template<class T> requires requires { T::StaticClass(); }
	static auto GetDeferredStaticClass()
	{
		return &T::StaticRegisterClass;
	}

	template<class T> requires
		std::is_pointer_v<T> &&
		requires { std::remove_pointer_t<T>::StaticClass(); }
	static auto GetDeferredStaticClass()
	{
		return &std::remove_pointer_t<T>::StaticRegisterClass;
	}

	template<class T> requires
		(!requires { T::StaticClass(); }) &&
		(!requires { std::remove_pointer_t<T>::StaticClass(); })
	static auto GetDeferredStaticClass()
	{
		static Type* DeferredClass = GetStaticClass<T>();
		return &DeferredClass;
	}

	static Type* FindStaticClass(std::wstring_view InFriendlyName);
	static std::span<Type*> FindAllSubclass(Type* BaseClass);
	template<class T>
	static std::span<Type*> FindAllSubclass() { return FindAllSubclass(GetStaticClass<T>()); }

private:
	template<std::derived_from<SObject> TType> requires std::constructible_from<TType>
	static ObjectCtor GetConstructorFunctionBody(int32)
	{
		return +[]() -> SObject*
		{
			return gcnew TType();
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