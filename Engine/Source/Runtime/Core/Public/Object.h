// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "reflexpr.h"
#include "Misc/String.h"
#include "Reflection/ObjectMacros.h"
#include "Reflection/ClassTypeMetadata.h"
#include "Object.generated.h"

class Type;
class GC;
class PropertyInfo;

/// <summary>
/// Supports all classes in the CPP.REF class hierarchy and provides low-level services
/// to derived classes. This is the ultimate base class of all C++ classes that contains
/// generated sources; it is the root of the type hierarchy.
/// </summary>
SCLASS()
class CORE_API Object
{
	GENERATED_BODY()
	friend class GC;
	friend class Type;

private:
	Object(const Object&) = delete;
	Object& operator =(const Object&) = delete;

private:
	SPROPERTY()
	int64 InternalIndex = -1;
	uint8 bRoot : 1 = false;
	uint8 bMarking : 1 = false;

public:
	/// <summary>
	/// Initializes a new instance of the Object class.
	/// </summary>
	SCONSTRUCTOR()
	Object() noexcept;
	virtual ~Object() noexcept;

	/// <summary>
	/// Gets the Type of the current instance.
	/// </summary>
	/// <returns> The exact runtime type of the current instance. </returns>
	SFUNCTION()
	Type* GetType() noexcept;

	/// <summary>
	/// Returns a string that represents the current object.
	/// </summary>
	/// <returns> A string that represents the current object. </returns>
	SFUNCTION()
	virtual String ToString() noexcept;

	/// <summary>
	/// Add an object to the root set.
	/// This prevents the object and all its descendants from being deleted during garbage collection.
	/// </summary>
	SFUNCTION()
	void AddToRoot();

	/// <summary>
	/// Remove an object from the root set.
	/// </summary>
	SFUNCTION()
	void RemoveFromRoot();

	/// <summary>
	/// Checks to see if the object appears to be valid.
	/// </summary>
	SFUNCTION()
	bool IsValidLowLevel();

protected:
	/// <summary>
	/// Creates a shallow copy of the current Object.
	/// </summary>
	/// <returns> A shallow copy of the current Object. </returns>
	SFUNCTION()
	Object* MemberwiseClone();

protected:
	static Type* GenerateClassType(const libty::reflect::ClassTypeMetadata& meta);
	static Type* FindClass(const String& friendlyName);
};

#include "Type.h"