// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "TypeInfoMetadataGenerator.h"
#include <span>
#include <map>
#include <set>

/// <summary>
/// Represents type declarations: class types, interface types, value types, and enumeration types.
/// </summary>
class CORESOBJECT_API SType : implements SObject
{
	GENERATED_BODY(SType);

public:
	using MetadataGenerator = libty::Core::Reflection::TypeInfoMetadataGenerator;

private:
	struct StaticCollection
	{
		std::map<std::wstring, SType*, std::less<>> FullQualifiedNameView;
		std::map<SType*, std::set<SType*>> HierarchyView;
	};

	static StaticCollection* _staticCollection;

	MetadataGenerator _meta;
	std::vector<SFieldInfo*> _recursiveFields;
	std::vector<SAttributeClass*> _recursiveAttributes;

public:
	SType(MetadataGenerator&& generator);

	/// <summary>
	/// Gets the simply name of the type.
	/// </summary>
	virtual std::wstring_view GetName();

	/// <summary>
	/// Gets the type from which the current Type directly inherits.
	/// </summary>
	virtual SType* GetSuperType();

	/// <summary>
	/// Gets the fully qualified name of the type, including its namespace but not its assembly.
	/// </summary>
	virtual std::wstring_view GetFullQualifiedName();

	/// <summary>
	/// Returns all the fields of the current Type.
	/// </summary>
	virtual std::span<SFieldInfo* const> GetFields(bool bRecursive = true);

	/// <summary>
	/// Returns specified field of the current Type.
	/// </summary>
	virtual SFieldInfo* GetField(std::wstring_view fieldName, bool bRecursive = true);

	/// <summary>
	/// Return custom attributes applied to this Type.
	/// </summary>
	virtual std::span<SAttributeClass* const> GetCustomAttributes(bool bRecursive = true);

	/// <summary>
	/// Returns custom attribute that first item of match with specified type.
	/// </summary>
	virtual SAttributeClass* GetCustomAttribute(SType* attributeType, bool bRecursive = true);

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
	/// Gets a valud indicating whether the Type equals to specified type.
	/// </summary>
	/// <param name="compareType"> The type what to compare. </param>
	virtual bool IsA(SType* compareType);

	/// <summary>
	/// Gets a valud indicating whether the Type derived from specified type.
	/// </summary>
	/// <param name="baseType"> The type what to compare. </param>
	virtual bool IsDerivedFrom(SType* baseType);

	/// <summary>
	/// Instantiate specified type with default constructor.
	/// </summary>
	virtual SObject* Instantiate();

	/// <summary>
	/// Gets the Type with the specified full qualified name, performing a case-sensitive search.
	/// </summary>
	/// <param name="fullQualifiedName"> The full qualified name of the type to get. </param>
	/// <returns> The type with the specified name, if found; otherwise, null. </returns>
	static SType* GetType(std::wstring_view fullQualifiedName);

	/// <summary>
	/// Get all derived types from specified base type.
	/// </summary>
	/// <param name="baseType"> The base type of results. </param>
	static std::set<SType*> GetDerivedTypes(SType* baseType);

	template<class T>
	static SType* TypeOf() requires
		(!requires { { std::declval<T>().GetType() } -> std::same_as<SType*>; })
	{
		static SType StaticClass = SType(MetadataGenerator::GenerateNative<T>());
		return &StaticClass;
	}

	template<class T>
	static SType* TypeOf() requires
		requires { { std::declval<T>().GetType() } -> std::same_as<SType*>; }
	{
		return T::TypeId;
	}
};