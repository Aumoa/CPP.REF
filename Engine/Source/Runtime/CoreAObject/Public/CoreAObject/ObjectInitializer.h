// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AObject;
class AType;
class APackage;
struct NTypeCollection;

class COREAOBJECT_API NObjectInitializer
{
	friend AObject;
	friend AType;
	friend APackage;
	friend NTypeCollection;

	AType* ConstructType = nullptr;

public:
	static NObjectInitializer& Get();

private:
	NObjectInitializer& MarkInit(AType* InConstructType);
	AType* ConsumeConstructType();
};