// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"

class AAssembly;

class COREAOBJECT_API AType : public AObject
{
protected:
	AType()
	{
	}

public:
	virtual ~AType() noexcept
	{
	}

	virtual AAssembly* GetAssembly() const = 0;
};