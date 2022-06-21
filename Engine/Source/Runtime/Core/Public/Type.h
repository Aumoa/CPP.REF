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
	std::vector<libty::reflect::constructor_t> _constructors;

private:
	Type() noexcept;

public:
	virtual ~Type() noexcept override;

	SFUNCTION()
	virtual String ToString() const noexcept override;

	/// <summary>
	/// Gets the name of the current type.
	/// </summary>
	SFUNCTION()
	String GetName() const noexcept;

	SFUNCTION()
	std::vector<ConstructorInfo*> GetConstructors() const noexcept;
};