// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "SerializedLevelSpawn.h"
#include "Level.gen.h"

class AGameMode;
class World;

SCLASS()
class GAME_API Level : public GameObject
{
	GENERATED_BODY()

protected:
	SubclassOf<AGameMode> GameModeClass;
	std::vector<SerializedLevelSpawn> SerializedSpawns;

public:
	Level();

	World* GetWorld() noexcept;

	void SpawnGameMode() { SpawnGameModeAt(GetWorld()); }
	AGameMode* SpawnGameModeAt(World* InWorld);
	void SpawnLevelActors(World* InWorld);
};