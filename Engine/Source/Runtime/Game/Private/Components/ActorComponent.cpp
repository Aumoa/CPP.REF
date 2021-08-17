// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Components/ActorComponent.h"
#include "LogGame.h"
#include "GameFramework/Actor.h"
#include "Level/World.h"

void ActorComponent::ComponentTickFunction::ExecuteTick(std::chrono::duration<float> elapsedTime)
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

ActorComponent::ActorComponent() : Super()
	, PrimaryComponentTick(this)
{
}

void ActorComponent::TickComponent(std::chrono::duration<float> elapsedTime, ComponentTickFunction* tickFunction)
{
}

void ActorComponent::BeginPlay()
{
	_bHasBegunPlay = true;
}

void ActorComponent::EndPlay()
{
	_bHasBegunPlay = false;
}

void ActorComponent::SetActive(bool bActive)
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

void ActorComponent::RegisterComponentWithWorld(World* world)
{
	if (!_bIsRegistered)
	{
		world->RegisterTickFunction(&PrimaryComponentTick);
		world->RegisterComponent(this);
		_bIsRegistered = true;
	}
}

void ActorComponent::UnregisterComponent()
{
	if (_bIsRegistered)
	{
		World* const world = GetWorld();
		if (world != nullptr)
		{
			world->UnregisterTickFunction(&PrimaryComponentTick);
			world->UnregisterComponent(this);
		}
		_bIsRegistered = false;
	}
}