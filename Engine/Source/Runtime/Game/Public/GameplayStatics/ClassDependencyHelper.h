// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class GGameObject;
class World;
class Engine;

class GAME_API ClassDependencyHelper : virtual public Object
{
public:
	ClassDependencyHelper() = delete;

	static Engine* GetEngine(GGameObject* inWorldContext);
	static Engine* GetEngine(World* inWorld);
};