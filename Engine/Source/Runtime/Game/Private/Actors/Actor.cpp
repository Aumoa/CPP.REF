// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Actors/Actor.h"
#include "Actor.gen.cpp"

AActor::ActorTickFunction::ActorTickFunction(AActor* InTarget)
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

void AActor::TickComponent(const TimeSpan& InDeltaTime, ActorTickFunction* InTickFunction)
{
	if (InTickFunction == &PrimaryActorTick)
	{
		Tick(InDeltaTime);
	}
}

void AActor::Tick(const TimeSpan& InDeltaTime)
{
}