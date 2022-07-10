// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "World.generated.h"

class Engine;
class LevelTick;

SCLASS()
class GAME_API World : virtual public Object
{
	GENERATED_BODY()

private:
	SPROPERTY()
	Engine* _engine = nullptr;
	SPROPERTY()
	LevelTick* _levelTick = nullptr;

public:
	World(Engine* engine);

	SFUNCTION()
	Engine* GetOuter();
	void DispatchWorldTick(float elapsedSeconds);
};