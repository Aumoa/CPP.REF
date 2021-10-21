// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/ActorComponent.h"
#include "LogGame.h"
#include "GameFramework/Actor.h"
#include "Level/World.h"
#include "Level/Level.h"
#include "Ticking/TickTaskLevelManager.h"

SActorComponent::SComponentTickFunction::SComponentTickFunction(SActorComponent* InTarget) : Super()
	, _ComponentTarget(InTarget)
{
}

void SActorComponent::SComponentTickFunction::ExecuteTick(float InDeltaTime)
{
	if (_ComponentTarget->HasBegunPlay() && _ComponentTarget->IsActive())
	{
		_ComponentTarget->TickComponent(InDeltaTime, this);
	}
}

SActorComponent* SActorComponent::SComponentTickFunction::GetTarget() const
{
	return _ComponentTarget;
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

void SActorComponent::BeginPlay()
{
	_bHasBegunPlay = true;
}

void SActorComponent::EndPlay()
{
	_bHasBegunPlay = false;
}

void SActorComponent::Tick(float InDeltaTime)
{
}

AActor* SActorComponent::GetOwner()
{
	return _OwnerPrivate;
}

void SActorComponent::SetActive(bool bActive)
{
	if (_bActive != bActive)
	{
		_bActive = bActive;
		PrimaryComponentTick.SetTickFunctionEnable(bActive);
		if (_bActive)
		{
			Activated.Invoke();
		}
		else
		{
			Inactivated.Invoke();
		}
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
	}

	_bRegistered = true;
}

void SActorComponent::UnregisterComponent()
{
	if (_bRegistered)
	{
		SWorld* World = GetWorld();
		SLevel* Level = World->GetLevel();
		RegisterAllTickFunctions(Level, false);
	}

	_bRegistered = false;
}

bool SActorComponent::IsRegistered()
{
	return _bRegistered;
}

void SActorComponent::MarkOwner()
{
	SObject* Outer = GetOuter();
	if (auto* IsActor = Cast<AActor>(Outer); IsActor)
	{
		_OwnerPrivate = IsActor;
	}
	else if (auto* IsComponent = Cast<SActorComponent>(Outer); IsComponent)
	{
		_OwnerPrivate = IsComponent->GetOwner();
	}
	else
	{
		_OwnerPrivate = nullptr;
	}
}

void SActorComponent::RegisterAllTickFunctions(SLevel* InLevel, bool bRegister)
{
	if (bRegister)
	{
		if (!PrimaryComponentTick.IsTickFunctionRegistered() && PrimaryComponentTick.bCanEverTick)
		{
			bool bEnabled = PrimaryComponentTick.bStartWithTickEnabled && PrimaryComponentTick.IsTickFunctionEnabled();
			PrimaryComponentTick.SetTickFunctionEnable(bEnabled);

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