// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "Reflection/ReflectionTraits.h"
#include "Type.generated.h"

class ConstructorInfo;

/// <summary>
/// Represents type declarations: SCLASS types, SINTERFACE types, SSTRUCT types, and SENUM types.
/// </summary>
SCLASS()
class CORE_API Type : virtual public Object
{
	GENERATED_BODY()
	friend class Object;

private:
	SPROPERTY()
	String _name;
	SPROPERTY()
	std::vector<std::unique_ptr<ConstructorInfo>> _constructors;

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
	/// Returns all the public constructors defined for the current System.Type.
	/// </summary>
	SFUNCTION()
	std::vector<ConstructorInfo*> GetConstructors() noexcept;
};