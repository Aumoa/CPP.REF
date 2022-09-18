// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Levels/GameStarterLevel.h"
#include "Actors/AylaGameMode.h"
#include "Actors/SampleActor.h"
#include "GameStarterLevel.gen.cpp"

GameStarterLevel::GameStarterLevel()
{
	GameModeClass = typeof(AAylaGameMode);

	SerializedSpawns.emplace_back(SerializedLevelSpawn
	{
		.ActorClass = ASampleActor::StaticClass()
	});
}