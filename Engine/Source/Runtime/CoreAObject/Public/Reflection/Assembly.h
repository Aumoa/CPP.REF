// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"

class AType;

class COREAOBJECT_API AAssembly : public AObject
{
	AYLA_DECLARE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, Assembly);
	AYLA_DECLARE_CLASS_TYPEDEFS(Engine, CoreAObject, Assembly);

protected:
	AAssembly();

public:
	virtual ~AAssembly() noexcept override;

	virtual String GetName() const = 0;
	virtual String GetNamespace() const = 0;
	virtual String GetFullName() const = 0;
	virtual std::span<AType* const> GetTypes() const = 0;
};