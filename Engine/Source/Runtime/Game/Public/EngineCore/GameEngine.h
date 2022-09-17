// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameEngine.generated.h"

class World;

SCLASS()
class GAME_API GameEngine : public Engine
{
	GENERATED_BODY()

private:
	SPROPERTY()
	World* GameWorld = nullptr;

public:
	GameEngine();

	virtual void Init() override;
	virtual void Start(GameInstance* Instance) override;

private:
	World* SpawnGameWorld();
};