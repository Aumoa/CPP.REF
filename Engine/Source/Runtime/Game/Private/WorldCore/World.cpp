// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "WorldCore/World.h"
#include "EngineCore/GameEngine.h"
#include "Ticking/LevelTick.h"
#include "WorldCore/Level.h"
#include "Actors/GameMode.h"
#include "World.gen.cpp"

World::World()
{
	LevelTick = NewObject<::LevelTick>();
}

void World::DispatchWorldTick(const TimeSpan& InDeltaTime)
{
	LevelTick->BeginFrame();
	LevelTick->IncrementalDispatchTick(ETickingGroup::PrePhysics, InDeltaTime);
	LevelTick->IncrementalDispatchTick(ETickingGroup::DuringPhysics, InDeltaTime);
	LevelTick->IncrementalDispatchTick(ETickingGroup::PostPhysics, InDeltaTime);
	LevelTick->IncrementalDispatchTick(ETickingGroup::PostUpdateWork, InDeltaTime);
	LevelTick->EndFrame();
}

void World::BrowseLevel(SubclassOf<Level> InLevelClass)
{
	if (PersistentLevel)
	{
		// Do unload level.
	}

	PersistentLevel = NewObject(InLevelClass);
	AGameMode* GameMode = PersistentLevel->SpawnGameModeAt(this);
	APlayerController* PlayerController = GameMode->SpawnPlayerController(this);
}

AActor* World::SpawnActor(SubclassOf<AActor> InActorClass, const String& ActorName)
{
	return NewObject(InActorClass, ActorName);
}

GameEngine* World::GetEngine() noexcept
{
	return Cast<GameEngine>(GetOuter());
}