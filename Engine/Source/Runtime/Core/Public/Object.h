// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "reflexpr.h"
#include "Misc/String.h"
#include "Reflection/ObjectMacros.h"
#include "Object.generated.h"

class Type;

SCLASS()
class CORE_API Object
{
	GENERATED_BODY()

public:
	Object() noexcept;
	virtual ~Object() noexcept;

	template<class TSelf>
	Type* GetType(this TSelf&&) noexcept;
	virtual String ToString() const noexcept;
};