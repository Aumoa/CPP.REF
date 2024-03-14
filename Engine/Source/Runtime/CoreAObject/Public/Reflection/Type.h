// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"

class AAssembly;

class COREAOBJECT_API AType : public AObject
{
	AYLA_DECLARE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, Type);
	AYLA_DECLARE_CLASS_TYPEDEFS(Engine, CoreAObject, Type)

protected:
	AType();

public:
	virtual ~AType() noexcept override;

	virtual AAssembly* GetAssembly() const = 0;
	virtual String GetName() const = 0;
	virtual AType* GetBaseType() const = 0;
};