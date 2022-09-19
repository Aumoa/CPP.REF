// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Components/ActorComponent.h"
#include "Actors/Actor.h"
#include "ActorComponent.gen.cpp"

ActorComponent::ComponentTickFunction::ComponentTickFunction(ActorComponent* Target)
	: ComponentTarget(Target)
{
}

void ActorComponent::ComponentTickFunction::ExecuteTick(const TimeSpan& InDeltaTime)
{
	if (ensure(ComponentTarget.IsValid() && ComponentTarget->HasBegunPlay()) && ComponentTarget->IsActive())
	{
		ComponentTarget->TickComponent(InDeltaTime, this);
	}
}

ActorComponent::ActorComponent()
	: PrimaryComponentTick(this)
{
}

void ActorComponent::TickComponent(const TimeSpan& InDeltaTime, ComponentTickFunction* InTickFunction)
{
	if (InTickFunction == &PrimaryComponentTick)
	{
		Tick(InDeltaTime);
	}
}

AActor* ActorComponent::GetOwner() noexcept
{
	return Cast<AActor>(GetOuter());
}

World* ActorComponent::GetWorld() noexcept
{
	return GetOwner()->GetWorld();
}

void ActorComponent::RegisterComponent()
{
}

void ActorComponent::UnregisterComponent()
{
}

void ActorComponent::Tick(const TimeSpan& InDeltaTime)
{
}