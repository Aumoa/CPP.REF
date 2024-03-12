// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"

class AType;

class COREAOBJECT_API AParameterInfo : public AObject
{
protected:
	AParameterInfo()
	{
	}

public:
	virtual ~AParameterInfo() noexcept
	{
	}

	virtual String GetName() const = 0;
	virtual AType* GetParameterType() const = 0;
	virtual int32 GetPosition() const = 0;
};