// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Actors/Actor.h"
#include "WorldCore/World.h"
#include "WorldCore/Level.h"
#include "Ticking/LevelTick.h"
#include "Components/SceneComponent.h"
#include "Actor.gen.cpp"

AActor::ActorTickFunction::ActorTickFunction(AActor* InTarget)
	: Target(InTarget)
{
}

AActor* AActor::ActorTickFunction::GetTarget() const noexcept
{
	return Target.Get();
}

void AActor::ActorTickFunction::ExecuteTick(const TimeSpan& InDeltaTime)
{
	if (ensure(Target.IsValid() && Target->HasBegunPlay()) && Target->IsActive())
	{
		Target->TickComponent(InDeltaTime, this);
	}
}

AActor::AActor()
	: PrimaryActorTick(this)
{
	MarkObjectFlags(EObjectFlags::Active);
}

void AActor::RegisterAllTickFunctions(World* InWorld)
{
	LevelTick* LevelTick = InWorld->GetLevelTick();
	LevelTick->AddTickFunction(&PrimaryActorTick);
}

void AActor::UnregisterAllTickFunctions(World* InWorld)
{
	LevelTick* LevelTick = InWorld->GetLevelTick();
	LevelTick->RemoveTickFunction(&PrimaryActorTick);
}

void AActor::TickComponent(const TimeSpan& InDeltaTime, ActorTickFunction* InTickFunction)
{
	if (InTickFunction == &PrimaryActorTick)
	{
		Tick(InDeltaTime);
	}
}

Level* AActor::GetLevel() noexcept
{
	return Cast<Level>(GetOuter());
}

World* AActor::GetWorld() noexcept
{
	if (Level* Level = GetLevel())
	{
		return Level->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

void AActor::SetRootComponent(SceneComponent* InRootComponent)
{
	RootComponent = InRootComponent;
}

SceneComponent* AActor::GetRootComponent() noexcept
{
	return RootComponent;
}

void AActor::GetComponents(std::list<ActorComponent*>& OutComponents, SubclassOf<ActorComponent> InComponentClass)
{
	if (RootComponent && RootComponent->GetType()->IsDerivedFrom(InComponentClass.Get()))
	{
		OutComponents.emplace_back(RootComponent);
	}
}

void AActor::RegisterActor()
{
	MarkObjectFlags(EObjectFlags::Registered | EObjectFlags::BegunPlay);

	RegisterAllTickFunctions(GetWorld());
	BeginPlay();
}

void AActor::UnregisterActor()
{
	EndPlay();
	UnregisterAllTickFunctions(GetWorld());

	UnmarkObjectFlags(EObjectFlags::Registered | EObjectFlags::BegunPlay);
}

void AActor::Destroy()
{
	GetWorld()->DestroyActor(this);
}

void AActor::BeginPlay()
{
}

void AActor::EndPlay()
{
}

void AActor::Tick(const TimeSpan& InDeltaTime)
{
}