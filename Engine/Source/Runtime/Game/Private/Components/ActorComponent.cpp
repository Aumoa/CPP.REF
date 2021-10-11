// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Components/ActorComponent.h"
#include "LogGame.h"
#include "GameFramework/Actor.h"
#include "Level/World.h"

void SActorComponent::SComponentTickFunction::ExecuteTick(float elapsedTime)
{
	if (_target == nullptr)
	{
		SE_LOG(LogTicking, Error, L"Target is nullptr.");
		return;
	}

	AActor* const owner = _target->GetOwner();
	if (owner == nullptr)
	{
		SE_LOG(LogTicking, Error, L"Target have not any owner actor.");
		return;
	}

	if (owner->HasBegunPlay() && owner->IsActive() &&
		_target->HasBegunPlay() && _target->IsActive())
	{
		_target->TickComponent(elapsedTime, this);
	}
}

SActorComponent::SActorComponent() : Super()
	, PrimaryComponentTick(this)
{
}

void SActorComponent::TickComponent(float elapsedTime, SComponentTickFunction* tickFunction)
{
	if (tickFunction == &PrimaryComponentTick)
	{
		Tick(elapsedTime);
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

void SActorComponent::Tick(float elapsedTime)
{
}

void SActorComponent::SetActive(bool bActive)
{
	if (_bActive != bActive)
	{
		_bActive = bActive;
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
	AActor* MyOwner = GetOwner();
	if (ensure(MyOwner))
	{
		if (SWorld* OwnerWorld = MyOwner->GetWorld(); ensure(OwnerWorld))
		{
			RegisterComponentWithWorld(OwnerWorld);
		}
	}
}

void SActorComponent::RegisterComponentWithWorld(SWorld* world)
{
	if (!_bRegistered)
	{
		world->RegisterTickFunction(&PrimaryComponentTick);
		world->RegisterComponent(this);
		_bRegistered = true;
	}
}

void SActorComponent::UnregisterComponent()
{
	if (_bRegistered)
	{
		SWorld* const world = GetWorld();
		if (world != nullptr)
		{
			world->UnregisterTickFunction(&PrimaryComponentTick);
			world->UnregisterComponent(this);
		}
		_bRegistered = false;
	}
}

bool SActorComponent::IsRegistered()
{
	return _bRegistered;
}