// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "reflexpr.h"
#include "Misc/String.h"
#include "Reflection/ObjectMacros.h"
#include "Object.generated.h"

class Type;

/// <summary>
/// Supports all classes in the CPP.REF class hierarchy and provides low-level services
/// to derived classes. This is the ultimate base class of all C++ classes that contains
/// generated sources; it is the root of the type hierarchy.
/// </summary>
SCLASS()
class CORE_API Object
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Initializes a new instance of the Object class.
	/// </summary>
	Object() noexcept;
	virtual ~Object() noexcept;

	/// <summary>
	/// Gets the Type of the current instance.
	/// </summary>
	/// <typeparam name="TSelf"> Type of this class. </typeparam>
	/// <returns> The exact runtime type of the current instance. </returns>
	template<class TSelf>
	Type* GetType(this TSelf&&) noexcept;

	/// <summary>
	/// Returns a string that represents the current object.
	/// </summary>
	/// <returns> A string that represents the current object. </returns>
	virtual String ToString() const noexcept;

protected:
	/// <summary>
	/// Creates a shallow copy of the current Object.
	/// </summary>
	/// <returns> A shallow copy of the current Object. </returns>
	Object MemberwiseClone() const;
};