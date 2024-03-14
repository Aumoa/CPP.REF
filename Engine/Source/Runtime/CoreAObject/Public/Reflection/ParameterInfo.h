// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"

class AType;

class COREAOBJECT_API AParameterInfo : public AObject
{
	AYLA_DECLARE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, ParameterInfo);
	AYLA_DECLARE_CLASS_TYPEDEFS(Engine, CoreAObject, ParameterInfo);

protected:
	AParameterInfo();

public:
	virtual ~AParameterInfo() noexcept override;

	virtual String GetName() const = 0;
	virtual AType* GetParameterType() const = 0;
	virtual int32 GetPosition() const = 0;
};