// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "TypeInfoMetadataGenerator.h"
#include <span>
#include <map>
#include <set>

namespace libty::inline Core
{
	class GarbageCollector;
	namespace Reflection
	{
		class SEnum;
	}

	/// <summary>
	/// Represents type declarations: class types, interface types, value types, and enumeration types.
	/// </summary>
	class CORESOBJECT_API SType : implements(SObject)
	{
		GENERATED_BODY(SType);
		friend class ::libty::Core::GarbageCollector;
		friend class ::libty::Core::Reflection::SEnum;

	public:
		using MetadataGenerator = libty::Core::Reflection::TypeInfoMetadataGenerator;

	private:
		struct StaticCollection
		{
			std::set<SType*> AllTypes;
			std::map<String, SType*, std::less<>> FullQualifiedNameView;
			std::map<SType*, std::set<SType*>> HierarchyView;
		};

		static StaticCollection* _staticCollection;

		MetadataGenerator _meta;

		Spinlock _lock;
		bool _cached_private = false;
		SType* _baseType = nullptr;
		std::vector<Reflection::SFieldInfo*> _recursiveFields;
		std::vector<Reflection::SMethodInfo*> _recursiveMethods;
		std::vector<SClassAttribute*> _recursiveAttributes;

	public:
		SType(MetadataGenerator&& generator);

		/// <summary>
		/// Gets the simply name of the type.
		/// </summary>
		virtual StringView GetName();

		/// <summary>
		/// Gets the type from which the current Type directly inherits.
		/// </summary>
		virtual SType* GetSuperType();

		/// <summary>
		/// Gets the fully qualified name of the type, including its namespace but not its assembly.
		/// </summary>
		virtual StringView GetFullQualifiedName();

		/// <summary>
		/// Returns all the fields of the current Type.
		/// </summary>
		virtual std::span<Reflection::SFieldInfo* const> GetFields(bool bRecursive = true);

		/// <summary>
		/// Returns specified field of the current Type.
		/// </summary>
		virtual Reflection::SFieldInfo* GetField(StringView fieldName, bool bRecursive = true);

		/// <summary>
		/// Returns all the methods of the current Type.
		/// </summary>
		virtual std::span<Reflection::SMethodInfo* const> GetMethods(bool bRecursive = true);

		/// <summary>
		/// Returns specified method of the current Type.
		/// </summary>
		virtual Reflection::SMethodInfo* GetMethod(StringView methodName, bool bRecursive = true);

		/// <summary>
		/// Return custom attributes applied to this Type.
		/// </summary>
		virtual std::span<SClassAttribute* const> GetCustomAttributes(bool bRecursive = true);

		/// <summary>
		/// Returns custom attribute that first item of match with specified type.
		/// </summary>
		virtual SClassAttribute* GetCustomAttribute(SType* attributeType, bool bRecursive = true);

		/// <summary>
		/// Returns custom attribute that first item of match with specified type.
		/// </summary>
		template<std::derived_from<SClassAttribute> T>
		T* GetCustomAttribute(bool bRecursive = true)
		{
			return Cast<T>(GetCustomAttribute(TypeOf<T>(), bRecursive));
		}

		/// <summary>
		/// Returns imlemented interfaces collection.
		/// </summary>
		virtual std::span<SType* const> GetInterfaces();

		/// <summary>
		/// Gets the Assembly in which the type is declared.
		/// </summary>
		virtual Reflection::SAssembly* GetAssembly();

		/// <summary>
		/// Returns the hash code for this type.
		/// </summary>
		virtual size_t GetHashCode();

		/// <summary>
		/// Gets a value indicating whether the Type is a value type.
		/// </summary>
		virtual bool IsValueType();

		/// <summary>
		/// Gets a value indicating whether the Type is a native type.
		/// </summary>
		virtual bool IsNativeType();

		/// <summary>
		/// Gets a value indicating whether the Type is a interface type.
		/// </summary>
		virtual bool IsInterfaceType();

		/// <summary>
		/// Gets a value indicating whether the Type is a enum type.
		/// </summary>
		virtual bool IsEnum();

		/// <summary>
		/// Gets a value indicating whether the Type equals to specified type.
		/// </summary>
		/// <param name="compareType"> The type what to compare. </param>
		virtual bool IsA(SType* compareType);

		/// <summary>
		/// Gets a value indicating whether the Type equals to specified type.
		/// </summary>
		/// <typeparam name="T"> The type what to compare. </typeparam>
		template<class T>
		bool IsA()
		{
			return IsA(TypeOf<T>());
		}

		/// <summary>
		/// Gets a value indicating whether the Type derived from specified type.
		/// </summary>
		/// <param name="baseType"> The type what to compare. </param>
		virtual bool IsDerivedFrom(SType* baseType);

		/// <summary>
		/// Gets a value indicating whether the Type derived from specified type.
		/// </summary>
		/// <typeparam name="T"> The type what to compare. </typeparam>
		template<class T>
		bool IsDerivedFrom()
		{
			return IsDerivedFrom(TypeOf<T>());
		}

		/// <summary>
		/// Gets a value indicating whether the Type derived from specified type or implements interface.
		/// </summary>
		/// <param name="baseTypeOrInterface"> The type what to check. </param>
		virtual bool IsImplemented(SType* baseTypeOrInterface);

		/// <summary>
		/// Gets a value indicating whether the Type is constructible without any parameters.
		/// </summary>
		virtual bool IsConstructible();

		/// <summary>
		/// Instantiate specified type with default constructor.
		/// </summary>
		virtual SObject* Instantiate();

		/// <summary>
		/// Gets the Type with the specified full qualified name, performing a case-sensitive search.
		/// </summary>
		/// <param name="fullQualifiedName"> The full qualified name of the type to get. </param>
		/// <returns> The type with the specified name, if found; otherwise, null. </returns>
		static SType* GetType(StringView fullQualifiedName);

		/// <summary>
		/// Get all types.
		/// </summary>
		static const std::set<SType*>& GetAllTypes();

		/// <summary>
		/// Get all derived types from specified base type.
		/// </summary>
		/// <param name="baseType"> The base type of results. </param>
		static const std::set<SType*>& GetDerivedTypes(SType* baseType);

	private:
		template<class T>
		struct TypeOfImpl
		{
			static SType* Generate() requires
				(!requires { { std::declval<T>().GetType() } -> std::same_as<SType*>; }) &&
				(!std::same_as<T, void>)
			{
				static SType StaticClass = SType(MetadataGenerator::GenerateNative<T>());
				return &StaticClass;
			}

			static SType* Generate() requires
				requires { { std::declval<T>().GetType() } -> std::same_as<SType*>; }
			{
				return T::TypeId;
			}

			static SType* Generate() requires std::same_as<T, void>
			{
				return nullptr;
			}
		};

		template<template<class...> class TGenericClass, class... TArguments> requires
			IGenericClass<TGenericClass, TArguments...>
		struct TypeOfImpl<TGenericClass<TArguments...>>
		{
			static SType* Generate()
			{
				static SType StaticClass = SType(MetadataGenerator::GenerateNative<TGenericClass<>>());
				return &StaticClass;
			}
		};

	public:
		template<class T>
		static SType* TypeOf() requires
			requires { std::declval<TypeOfImpl<T>>(); }
		{
			return TypeOfImpl<T>::Generate();
		}

	private:
		void _Cache_private();
	};
}