// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "Reflection/ReflectionTraits.h"
#include "Type.generated.h"

class ConstructorInfo;
class PropertyInfo;
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
	SPROPERTY()
	String _name;
	std::unique_ptr<Type> _base;
	std::vector<std::unique_ptr<ConstructorInfo>> _constructors;
	std::vector<std::unique_ptr<PropertyInfo>> _properties;
	std::vector<PropertyInfo*> _gcproperties;
	EClassType _class_type;

private:
	Type() noexcept;
	void GenerateClass(const libty::reflect::ClassTypeMetadata& meta);

public:
	virtual ~Type() noexcept override;
	
	/// <summary>
	/// Gets the name of the current type.
	/// </summary>
	SFUNCTION()
	virtual String ToString() noexcept override;

	/// <summary>
	/// Gets the name of the current type.
	/// </summary>
	SFUNCTION()
	String GetName() noexcept;

	/// <summary>
	/// Gets a value indicating whether the Type is a class.
	/// </summary>
	bool IsClass() noexcept;

	/// <summary>
	/// Gets the type from which the current Type directly inherits.
	/// </summary>
	SFUNCTION()
	Type* GetBaseType() noexcept;

	/// <summary>
	/// Returns all the public constructors defined for the current System.Type.
	/// </summary>
	SFUNCTION()
	std::vector<ConstructorInfo*> GetConstructors() noexcept;

	/// <summary>
	/// Gets the properties of the current Type.
	/// </summary>
	SFUNCTION()
	std::vector<PropertyInfo*> GetProperties() noexcept;

public:
	template<class T>
	Type* TypeOf() requires
		requires { { T::StaticClass() } -> std::same_as<Type*>; }
	{
		return T::StaticClass();
	}
};

#define typeof(x) Type::TypeOf<x>();