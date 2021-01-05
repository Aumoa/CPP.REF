// Copyright 2020 Aumoa.lib. All right reserved.

#include "World.h"

#include "Level.h"
#include "Framework/Actor.h"
#include "SceneRendering/Scene.h"
#include "Components/PrimitiveComponent.h"

using namespace std;
using namespace std::chrono;

World::World()
{
	scene = NewObject<Scene>();
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
	Tick_Group(deltaTime, TickingGroup::PrePhysics);
	Tick_Group(deltaTime, TickingGroup::DuringPhysics);
	Tick_Group(deltaTime, TickingGroup::PostPhysics);
	Tick_Group(deltaTime, TickingGroup::PostUpdateWork);
	scene->Update();
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

Scene* World::GetScene() const
{
	return scene.Get();
}

void World::Tick_Group(duration<double> deltaTime, TickingGroup group)
{
	set<TickFunction*>& tickGroup = tickGroups[(size_t)group];

	for (auto& item : tickGroup)
	{
		item->ExecuteTick(deltaTime);
	}
}

AActor* World::SpawnActorInternal(TRefPtr<AActor> actor)
{
	AActor* actor_ptr = actor.Get();
	actors.push_back(move(actor));

	AddTickGroup(actor_ptr);
	AddSceneProxy(actor_ptr);

	actor_ptr->BeginPlay();

	return actor_ptr;
}

void World::AddTickGroup(AActor* actor_ptr)
{
	// Add to tick group.
	if (auto& tickFunction = actor_ptr->PrimaryActorTick; tickFunction.bCanEverTick)
	{
		auto tickGroup = tickFunction.TickGroup;
		tickGroups[(size_t)tickGroup].emplace(&tickFunction);
	}

	// Add all components to tick group.
	list<ActorComponent*> actorComponents = actor_ptr->GetComponents<ActorComponent>();
	for (auto& component : actorComponents)
	{
		if (auto& tickFunction = component->PrimaryComponentTick; tickFunction.bCanEverTick)
		{
			auto tickGroup = tickFunction.TickGroup;
			tickGroups[(size_t)tickGroup].emplace(&tickFunction);
		}
	}
}

void World::AddSceneProxy(AActor* actor_ptr)
{
	list<PrimitiveComponent*> primitiveComponents = actor_ptr->GetComponents<PrimitiveComponent>();
	
	for (auto& item : primitiveComponents)
	{
		if (item->HasDirtyMark())
		{
			item->ResolveDirtyState();
		}

		scene->AddScene(item);
	}
}