// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "WorldCore/World.h"
#include "EngineCore/GameEngine.h"
#include "Ticking/LevelTick.h"
#include "WorldCore/Level.h"
#include "Actors/GameMode.h"
#include "Rendering/RenderScene.h"
#include "Components/ActorComponent.h"
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
	IncrementalRegisterComponents();

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
		std::set<RefPtr<AActor>>& Actors = LevelActors[PersistentLevel];
		for (auto& ActorPtr : Actors)
		{
			if (auto* Actor = ActorPtr.Get(); Actor)
			{
				Actor->Destroy();
			}
		}

		PersistentLevel = nullptr;
	}

	PersistentLevel = NewObject(InLevelClass);

	AGameMode* GameMode = PersistentLevel->SpawnGameModeAt(this);
	GameMode->SpawnPlayerController(this);
	PersistentLevel->SpawnLevelActors(this);
}

void World::GetAllActorsOfClass(std::vector<AActor*>& OutActors, SubclassOf<AActor> InActorType) noexcept
{
	OutActors.reserve(OutActors.size() + LevelActors.size());

	for (auto& [Level, Actors] : LevelActors)
	{
		for (auto& ActorPtr : Actors)
		{
			if (auto* Actor = ActorPtr.Get(); Actor->GetType()->IsDerivedFrom(InActorType.Get()))
			{
				OutActors.emplace_back(Actor);
			}
		}
	}
}

AActor* World::SpawnActorAt(Level* InLevel, SubclassOf<AActor> InActorClass, const String& ActorName)
{
	AActor* Actor = InLevel->NewObject(InActorClass, ActorName);

	std::set<RefPtr<AActor>>& Actors = LevelActors[InLevel];
	Actors.emplace(Actor);

	Actor->GetComponents(PendingRegisterComponents, ActorComponent::StaticClass());
	PendingRegisterComponents.emplace_back(nullptr);

	return Actor;
}

void World::DestroyActor(AActor* InActor)
{
	Level* ActorLevel = InActor->GetLevel();
	checkf(ActorLevel, TEXT("Actor is not registered on any level."));

	InActor->UnregisterActor();

	std::list<ActorComponent*> UnregisterComponents;
	InActor->GetComponents(UnregisterComponents, ActorComponent::StaticClass());

	for (auto& Component : UnregisterComponents)
	{
		Component->UnregisterComponent();
	}

	InActor->UnmarkObjectFlags(EObjectFlags::Registered);

	std::set<RefPtr<AActor>>& Actors = LevelActors[ActorLevel];
	const bool bErased = Actors.erase(InActor) > 0;
	check(bErased);
}

GameEngine* World::GetEngine() noexcept
{
	return Cast<GameEngine>(GetOuter());
}

LevelTick* World::GetLevelTick() noexcept
{
	return LevelTick;
}

void World::IncrementalRegisterComponents(size_t NumRegisterComponents)
{
	check(NumRegisterComponents > 0);

	AActor* PreviousComponentActor = nullptr;
	while (NumRegisterComponents-- > 0 && PendingRegisterComponents.size() > 0)
	{
		ActorComponent* ComponentToRegister = PendingRegisterComponents.front();
		PendingRegisterComponents.erase(PendingRegisterComponents.begin());

		if (ComponentToRegister == nullptr)
		{
			if (PreviousComponentActor)
			{
				PreviousComponentActor->RegisterActor();
				PreviousComponentActor = nullptr;
			}

			++NumRegisterComponents;
			continue;
		}

		PreviousComponentActor = ComponentToRegister->GetOwner();
		check(PreviousComponentActor);

		ComponentToRegister->RegisterComponent();
	}

	if (PreviousComponentActor && PendingRegisterComponents.size() > 0 && PendingRegisterComponents.front() == nullptr)
	{
		PendingRegisterComponents.erase(PendingRegisterComponents.begin());
		PreviousComponentActor->RegisterActor();
		PreviousComponentActor = nullptr;
	}
}