﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "World.h"

#include "Level.h"
#include "Engine.h"
#include "Framework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/LightComponent.h"
#include "Logging/LogMacros.h"
#include "Logging/EngineLogCategory.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "SceneRendering/Scene.h"

using namespace std;
using namespace std::chrono;

DEFINE_STATS_GROUP(World);

World::World(GameInstance* gameInstance) : Super()
	, gameInstance(gameInstance)
	, localPlayerController(nullptr)
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

void World::Tick(Seconds deltaTime)
{
	QUICK_SCOPED_CYCLE_COUNTER(World, Tick);

	Tick_Ready();
	Tick_Group(deltaTime, TickingGroup::PrePhysics);
	Tick_Group(deltaTime, TickingGroup::DuringPhysics);
	Tick_Group(deltaTime, TickingGroup::PostPhysics);
	Tick_Group(deltaTime, TickingGroup::PostUpdateWork);
	scene->Update();
}

void World::RegisterComponent(GActorComponent* inComponent)
{
	if (GPrimitiveComponent* isPrimitive = Cast<GPrimitiveComponent>(inComponent); isPrimitive != nullptr)
	{
		scene->AddPrimitive(isPrimitive);
	}

	if (GLightComponent* isLight = Cast<GLightComponent>(inComponent); isLight != nullptr)
	{
		scene->AddLight(isLight);
	}
}

GLevel* World::LoadLevel(TSubclassOf<GLevel> loadLevel)
{
	if (currentLevel.IsValid)
	{
		currentLevel->UnloadLevel();
		currentLevel->SetWorld(nullptr);
		OnUnloadLevel();
	}

	currentLevel = loadLevel.Instantiate();
	if (!currentLevel.IsValid)
	{
		SE_LOG(LogEngine, Fatal, L"Cannot load level.");
		return nullptr;
	}

	if (currentLevel.IsValid)
	{
		currentLevel->SetWorld(this);
		currentLevel->LoadLevel();
		OnLoadLevel();
	}

	return currentLevel.Get();
}

GameInstance* World::GetGameInstance() const
{
	return gameInstance;
}

Scene* World::GetScene() const
{
	return scene.Get();
}

GLevel* World::GetCurrentLevel() const
{
	return currentLevel.Get();
}

void World::Tick_Ready()
{
	for (auto& tickGroup : actualTickGroups)
	{
		tickGroup.clear();
	}

	for (auto& tickGroup : tickGroups)
	{
		for (auto& item : tickGroup)
		{
			item->ReadyTick();

			if (item->bNeedActualGroup)
			{
				actualTickGroups[(size_t)item->ActualTickGroup].emplace(item);
			}
		}
	}
}

void World::Tick_Group(Seconds deltaTime, TickingGroup group)
{
	set<TickFunction*>& tickGroup = tickGroups[(size_t)group];

	for (auto& item : tickGroup)
	{
		if (!item->bCompleteTickThisFrame && !item->bNeedActualGroup)
		{
			item->ExecuteTick(deltaTime);
		}
	}

	set<TickFunction*>& actualTickGroup = actualTickGroups[(size_t)group];

	for (auto& item : actualTickGroup)
	{
		if (!item->bCompleteTickThisFrame)
		{
			item->ExecuteTick(deltaTime);
		}
	}
}

AActor* World::SpawnActorInternal(TRefPtr<AActor> actor)
{
	AActor* actor_ptr = actor.Get();
	actors.push_back(move(actor));
	actor_ptr->SetWorld(this);

	AddTickGroup(actor_ptr);
	RegisterActorComponents(actor_ptr);

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
	list<GActorComponent*> actorComponents = actor_ptr->GetComponents<GActorComponent>();
	for (auto& component : actorComponents)
	{
		if (auto& tickFunction = component->PrimaryComponentTick; tickFunction.bCanEverTick)
		{
			auto tickGroup = tickFunction.TickGroup;
			tickGroups[(size_t)tickGroup].emplace(&tickFunction);
		}
	}
}

void World::RegisterActorComponents(AActor* actor_ptr)
{
	list<GActorComponent*> actorComponents = actor_ptr->GetComponents<GActorComponent>();
	for (auto& item : actorComponents)
	{
		RegisterComponent(item);
	}
}

void World::OnLoadLevel()
{
	for (size_t i = 0; i < actors.size(); ++i)
	{
		if (!actors[i]->HasBegunPlay)
		{
			actors[i]->BeginPlay();
		}
	}
}

void World::OnUnloadLevel()
{
	for (size_t i = 0; i < actors.size(); ++i)
	{
		if (!actors[i]->HasBegunPlay)
		{
			actors[i]->EndPlay();
		}
	}
}

void World::RegisterPlayerController(APlayerController* inPlayerController)
{
	localPlayerController = inPlayerController;
	scene = NewObject<Scene>(inPlayerController);
}