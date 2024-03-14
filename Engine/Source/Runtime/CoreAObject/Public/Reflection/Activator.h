// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class AType;
class AObject;

class Activator : public StaticClass
{
public:
	static AObject* CreateInstance(AType* classType);
};