// Copyright 2020 Aumoa.lib. All right reserved.

#include "World.h"

#include "Level.h"
#include "Framework/Actor.h"
#include "SceneRendering/Scene.h"
#include "Components/PrimitiveComponent.h"
#include "Logging/LogMacros.h"
#include "Logging/EngineLogCategory.h"

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

void World::Tick(Seconds deltaTime)
{
	Tick_Group(deltaTime, TickingGroup::PrePhysics);
	Tick_Group(deltaTime, TickingGroup::DuringPhysics);
	Tick_Group(deltaTime, TickingGroup::PostPhysics);
	Tick_Group(deltaTime, TickingGroup::PostUpdateWork);
	scene->Update();
}

Level* World::LoadLevel(TSubclassOf<Level> loadLevel)
{
	if (currentLevel.IsValid)
	{
		currentLevel->UnloadLevel();
	}

	currentLevel = loadLevel.Instantiate();
	if (!currentLevel.IsValid)
	{
		SE_LOG(LogEngine, Fatal, L"Cannot load level.");
		return nullptr;
	}

	if (currentLevel.IsValid)
	{
		currentLevel->world = this;
		currentLevel->LoadLevel();
	}

	return currentLevel.Get();
}

Scene* World::GetScene() const
{
	return scene.Get();
}

Level* World::GetCurrentLevel() const
{
	return currentLevel.Get();
}

void World::Tick_Group(Seconds deltaTime, TickingGroup group)
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
	actor_ptr->SetWorld(this);

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