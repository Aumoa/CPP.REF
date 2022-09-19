// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Actors/Actor.h"
#include "WorldCore/World.h"
#include "Ticking/LevelTick.h"
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

World* AActor::GetWorld() noexcept
{
	return Cast<World>(GetOuter());
}

void AActor::SetRootComponent(SceneComponent* InRootComponent)
{
	RootComponent = InRootComponent;
}

SceneComponent* AActor::GetRootComponent() noexcept
{
	return RootComponent;
}

void AActor::DispatchBeginPlay(World* InWorld)
{
	check(bHasBegunPlay == false);
	bHasBegunPlay = true;
}

void AActor::DispatchEndPlay(World* InWorld)
{
	check(bHasBegunPlay);
	bHasBegunPlay = false;
}

void AActor::Tick(const TimeSpan& InDeltaTime)
{
}