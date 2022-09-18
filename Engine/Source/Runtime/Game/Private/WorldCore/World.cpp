// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "WorldCore/World.h"
#include "EngineCore/GameEngine.h"
#include "Ticking/LevelTick.h"
#include "WorldCore/Level.h"
#include "Actors/GameMode.h"
#include "Rendering/RenderScene.h"
#include "World.gen.cpp"

World::World()
{
	LevelTick = NewObject<::LevelTick>();
	ScenePrivate = std::make_unique<RenderScene>(this);
}

World::~World() noexcept
{
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
	GameMode->SpawnPlayerController(this);
	PersistentLevel->SpawnLevelActors(this);
}

AActor* World::SpawnActor(SubclassOf<AActor> InActorClass, const String& ActorName)
{
	AActor* Actor = NewObject(InActorClass, ActorName);
	Actor->RegisterAllTickFunctions(this);
	Actors.emplace_back(Actor);
	Actor->DispatchBeginPlay(this);
	return Actor;
}

GameEngine* World::GetEngine() noexcept
{
	return Cast<GameEngine>(GetOuter());
}

LevelTick* World::GetLevelTick() noexcept
{
	return LevelTick;
}