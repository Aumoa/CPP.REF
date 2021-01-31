// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IEngineTick : virtual public Object
{
	virtual void Initialize() = 0;
	virtual void PostInitialize() = 0;
	virtual void Tick() = 0;
	virtual void Shutdown() = 0;
};