// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "WorldCore/Level.h"
#include "WorldCore/World.h"
#include "Actors/GameMode.h"
#include "Level.gen.cpp"

Level::Level() : Super()
{
	GameModeClass = AGameMode::StaticClass();
}

World* Level::GetWorld() noexcept
{
	return Cast<World>(GetOuter());
}

AGameMode* Level::SpawnGameModeAt(World* InWorld)
{
	return InWorld->SpawnActorAt(this, GameModeClass);
}

void Level::SpawnLevelActors(World* InWorld)
{
	for (auto& SpawnInfo : SerializedSpawns)
	{
		InWorld->SpawnActorAt(this, SpawnInfo.ActorClass);
	}
}