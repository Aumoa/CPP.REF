// Copyright 2020 Aumoa.lib. All right reserved.

#include "World.h"

#include "Level.h"
#include "Framework/Actor.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace SC::Runtime::Game::Framework;
using namespace std;
using namespace std::chrono;

World::World()
{

}

World::~World()
{
	if (currentLevel.IsValid)
	{
		currentLevel->UnloadLevel();
		currentLevel.Reset();
	}
}

void World::Tick(duration<double> deltaTime)
{
	Tick_PrePhysics(deltaTime);
}

void World::LoadLevel(Level* loadLevel)
{
	if (currentLevel.IsValid)
	{
		currentLevel->UnloadLevel();
	}

	currentLevel = loadLevel;

	if (currentLevel.IsValid)
	{
		currentLevel->world = this;
		loadLevel->LoadLevel();
	}
}

void World::Tick_PrePhysics(duration<double> deltaTime)
{
	auto it = tickGroups.find(TickingGroup::PrePhysics);
	if (it == tickGroups.end())
	{
		return;
	}

	for (auto& item : it->second)
	{
		item->ExecuteTick(deltaTime);
	}
}

AActor* World::SpawnActorInternal(TRefPtr<AActor> actor)
{
	AActor* actor_ptr = actor.Get();
	actors.push_back(move(actor));

	// Add to tick group.
	if (actor_ptr->PrimaryActorTick.bCanEverTick)
	{
		auto tickGroup = actor_ptr->PrimaryActorTick.TickGroup;
		auto it = tickGroups.find(tickGroup);
		if (it == tickGroups.end())
		{
			it = tickGroups.insert({ tickGroup, { } }).first;
		}
		it->second.emplace_back(&actor_ptr->PrimaryActorTick);
	}

	return actor_ptr;
}