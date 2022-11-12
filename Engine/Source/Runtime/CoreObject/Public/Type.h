// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "Reflection/ReflectionTraits.h"
#include <functional>
#include "Type.gen.h"

class ConstructorInfo;
class PropertyInfo;
class MethodInfo;
class GC;

/// <summary>
/// Represents type declarations: SCLASS types, SINTERFACE types, SSTRUCT types, and SENUM types.
/// </summary>
SCLASS()
class CORE_API Type : virtual public Object
{
	GENERATED_BODY()
	friend class Object;
	friend class GC;

private:
	enum class EClassType
	{
		Class,
		Enum,
		Interface,
		Struct,
		Native,
	};

private:
	String _name;
	size_t _hash_code;
	Type* _base = nullptr;
	std::vector<Type*> _interfaces;
	std::vector<std::unique_ptr<ConstructorInfo>> _constructors;
	std::vector<std::unique_ptr<PropertyInfo>> _properties;
	std::vector<std::unique_ptr<MethodInfo>> _methods;
	std::vector<PropertyInfo*> _gcproperties;
	EClassType _class_type = EClassType::Class;
	Object* (*_converter)(void*) = nullptr;

	Type* _genericSource = nullptr;
	bool _isGeneric = false;
	std::vector<Type*> _genericArgs;

private:
	Type() noexcept;
	void GenerateClass(const libty::reflect::ClassTypeMetadata& meta);

public:
	virtual ~Type() noexcept override;
	
	/// <summary>
	/// Gets the name of the current type.
	/// </summary>
	virtual String ToString() noexcept override;

	/// <summary>
	/// Determines if the underlying system type of the current Type is the same as the specified Type.
	/// </summary>
	/// <param name="type"> The type to be compared of the current Type. </param>
	/// <returns> true if type is the same as current Type; otherwise, false. </returns>
	bool Equals(Type* type) noexcept;

	/// <summary>
	/// Gets the name of the current type.
	/// </summary>
	String GetName() noexcept;

	/// <summary>
	/// Gets a value indicating whether the Type is a class.
	/// </summary>
	bool IsClass() noexcept;

	/// <summary>
	/// Gets a value indicating whether the Type is a interface.
	/// </summary>
	bool IsInterface() noexcept;

	/// <summary>
	/// Gets a value indicating whether the Type is derived from target Type.
	/// </summary>
	bool IsDerivedFrom(Type* t) noexcept;

	/// <summary>
	/// Gets a value indicating whether the Type is implements the target interface Type.
	/// </summary>
	bool IsImplements(Type* t) noexcept;

	/// <summary>
	/// Gets the hash code indicating this type.
	/// </summary>
	size_t GetHashCode() noexcept;

	/// <summary>
	/// Gets the type from which the current Type directly inherits.
	/// </summary>
	Type* GetBaseType() noexcept;

	/// <summary>
	/// When overridden in a derived class, gets all the interfaces implemented or inherited by the current Type.
	/// </summary>
	std::vector<Type*> GetInterfaces() noexcept;

	/// <summary>
	/// Returns all the public constructors defined for the current System.Type.
	/// </summary>
	std::vector<ConstructorInfo*> GetConstructors() noexcept;

	/// <summary>
	/// Gets the properties of the current Type.
	/// </summary>
	std::vector<PropertyInfo*> GetProperties() noexcept;

	/// <summary>
	/// Gets the methods of the current Type.
	/// </summary>
	std::vector<MethodInfo*> GetMethods() noexcept;

	/// <summary>
	/// Gets a value indicating whether the current type is a generic type.
	/// </summary>
	bool IsGenericType() noexcept;

	/// <summary>
	/// Returns an vector of Type objects that represent the type arguments of a closed generic type or the type parameters of a generic type definition.
	/// </summary>
	std::vector<Type*> GetGenericArguments() noexcept;

	/// <summary>
	/// Returns a Type object that represents a generic type definition from which the current generic type can be constructed.
	/// </summary>
	Type* GetGenericTypeDefinition() noexcept;

	/// <summary>
	/// Convert the raw pointer to instance of Object type.
	/// </summary>
	Object* GetObject(void* ptr) noexcept;

public:
	template<class T>
	static Type* TypeOf() requires
		requires { { T::StaticClass() } -> std::same_as<Type*>; }
	{
		return T::StaticClass();
	}

	template<class T>
	static Type* TypeOf() requires
		std::is_pointer_v<T> &&
		requires { { std::remove_pointer_t<T>::StaticClass() } -> std::same_as<Type*>; }
	{
		return TypeOf<std::remove_pointer_t<T>>();
	}

	template<template<class...> class T>
	static Type* TypeOf();

	template<class T>
	static Type* TypeOf();

private:
	bool _cached = false;
	const std::vector<PropertyInfo*>& GetGCProperties();
};

namespace type_details
{
	template<class T>
	struct expose_template_source
	{
	};

	template<template<class...> class T, class... TArgs>
	struct expose_template_source<T<TArgs...>>
	{
		static Type* expose_type() noexcept
		{
			return Type::TypeOf<T>();
		}

		static std::vector<Type*> expose_args() noexcept
		{
			return std::vector{ Type::TypeOf<TArgs>()... };
		}
	};

	template<template<class...> class T>
	struct hash_code_generator
	{
		static size_t hash_code() noexcept
		{
			return typeid(hash_code_generator<T>).hash_code();
		}
	};

	template<class T>
	struct is_generic_type
	{
		static constexpr bool value = false;
	};

	template<template<class...> class T, class... TArgs>
	struct is_generic_type<T<TArgs...>>
	{
		static constexpr bool value = true;
	};
}

template<template<class...> class T>
static Type* Type::TypeOf()
{
	static Type* sPtr = []()
	{
		static Type sGenericType;
		sGenericType._hash_code = type_details::hash_code_generator<T>::hash_code();
		sGenericType._class_type = EClassType::Native;
		sGenericType._isGeneric = true;
		return &sGenericType;
	}();

	return sPtr;
}

template<class T>
static Type* Type::TypeOf()
{
	static Type* sPtr = []()
	{
		static Type sGenericType;

		sGenericType._name = String::FromLiteral(typeid(T).name());
		sGenericType._hash_code = typeid(T).hash_code();
		sGenericType._class_type = EClassType::Native;

		if constexpr (type_details::is_generic_type<T>::value)
		{
			sGenericType._genericSource = type_details::expose_template_source<T>::expose_type();
			sGenericType._isGeneric = true;
			sGenericType._genericArgs = type_details::expose_template_source<T>::expose_args();
		}

		return &sGenericType;
	}();

	return sPtr;
}

namespace libty::reflect
{
	template<class T>
	inline Type* get_class()
	{
		return Type::TypeOf<T>();
	}
}

#define typeof(x) Type::TypeOf<x>()