// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ITickableComponent.generated.h"

class World;

SINTERFACE()
interface GAME_API ITickableComponent : virtual public Object
{
	GENERATED_BODY()

public:
	virtual void RegisterAllTickFunctions(World* InWorld) = 0;
	virtual void UnregisterAllTickFunctions(World* InWorld) = 0;
};