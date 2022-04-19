// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Components/ActorComponent.h"
#include "LogGame.h"
#include "GameFramework/Actor.h"
#include "Level/World.h"
#include "Level/Level.h"
#include "Ticking/TickTaskLevelManager.h"

using namespace ::libty;

SActorComponent::SComponentTickFunction::SComponentTickFunction(SActorComponent* InTarget)
	: ComponentTarget(InTarget)
{
}

void SActorComponent::SComponentTickFunction::ExecuteTick(float InDeltaTime)
{
	if (ComponentTarget->HasBegunPlay() && ComponentTarget->IsActive())
	{
		ComponentTarget->TickComponent(InDeltaTime, this);
	}
}

SActorComponent* SActorComponent::SComponentTickFunction::GetTarget() const
{
	return ComponentTarget;
}

SActorComponent::SActorComponent() : Super()
	, PrimaryComponentTick(this)
{
}

void SActorComponent::TickComponent(float InDeltaTime, SComponentTickFunction* InTickFunction)
{
	if (InTickFunction == &PrimaryComponentTick)
	{
		Tick(InDeltaTime);
	}
}

void SActorComponent::Tick(float InDeltaTime)
{
}

SLevel* SActorComponent::GetLevel()
{
	if (AActor* Actor = GetOwner())
	{
		return Actor->GetLevel();
	}
	return nullptr;
}

SWorld* SActorComponent::GetWorld()
{
	if (AActor* Actor = GetOwner())
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

AActor* SActorComponent::GetOwner()
{
	return OwnerPrivate;
}

void SActorComponent::SetActive(bool bActive)
{
	if (bActive != bActive)
	{
		bActive = bActive;
		PrimaryComponentTick.SetTickFunctionEnable(bActive);
		if (bActive)
		{
			Activated.Broadcast();
		}
		else
		{
			Inactivated.Broadcast();
		}
	}
}

void SActorComponent::MarkOwner()
{
	SObject* Outer = GetOuter();
	if (auto* IsActor = Cast<AActor>(Outer); IsActor)
	{
		OwnerPrivate = IsActor;
	}
	else if (auto* IsComponent = Cast<SActorComponent>(Outer); IsComponent)
	{
		OwnerPrivate = IsComponent->GetOwner();
	}
	else
	{
		OwnerPrivate = nullptr;
	}
}

void SActorComponent::RegisterComponent()
{
	MarkOwner();

	AActor* MyOwner = GetOwner();
	if (ensure(MyOwner))
	{
		if (SWorld* OwnerWorld = MyOwner->GetWorld(); ensure(OwnerWorld))
		{
			RegisterComponentWithWorld(OwnerWorld);
		}
	}
}

void SActorComponent::RegisterComponentWithWorld(SWorld* InWorld)
{
	if (!IsRegistered())
	{
		SLevel* Level = InWorld->GetLevel();
		RegisterAllTickFunctions(Level, true);

		AActor* MyOwner = GetOwner();
		if (MyOwner->HasBegunPlay())
		{
			DispatchBeginPlay();
		}
	}

	bRegistered = true;
}

void SActorComponent::UnregisterComponent()
{
	DispatchEndPlay();

	if (bRegistered)
	{
		SWorld* World = GetWorld();
		SLevel* Level = World->GetLevel();
		RegisterAllTickFunctions(Level, false);
	}

	bRegistered = false;
}

bool SActorComponent::IsRegistered()
{
	return bRegistered;
}

void SActorComponent::DispatchBeginPlay()
{
	if (!HasBegunPlay())
	{
		BeginPlay();
	}
}

void SActorComponent::DispatchEndPlay()
{
	if (HasBegunPlay())
	{
		EndPlay();
	}
}

void SActorComponent::BeginPlay()
{
	bHasBegunPlay = true;
}

void SActorComponent::EndPlay()
{
	bHasBegunPlay = false;
}

void SActorComponent::RegisterAllTickFunctions(SLevel* InLevel, bool bRegister)
{
	if (bRegister)
	{
		if (!PrimaryComponentTick.IsTickFunctionRegistered())
		{
			STickTaskLevelManager* LevelTick = InLevel->GetLevelTick();
			LevelTick->AddTickFunction(&PrimaryComponentTick);
		}
	}
	else
	{
		if (PrimaryComponentTick.IsTickFunctionRegistered())
		{
			STickTaskLevelManager* LevelTick = InLevel->GetLevelTick();
			LevelTick->RemoveTickFunction(&PrimaryComponentTick);
		}
	}
}