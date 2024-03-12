// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"

class AType;

class COREAOBJECT_API AMemberInfo : public AObject
{
protected:
	AMemberInfo()
	{
	}

public:
	virtual ~AMemberInfo() noexcept
	{
	}

	virtual AType* GetDeclaringType() const = 0;
};