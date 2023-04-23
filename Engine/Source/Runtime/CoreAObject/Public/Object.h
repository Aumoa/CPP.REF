// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"
//#include "Object.generated.h"

ACLASS()
class COREAOBJECT_API AObject
{
	//GENERATED_BODY()

public:
	AObject();
	virtual ~AObject() noexcept;

	virtual String ToString();
};