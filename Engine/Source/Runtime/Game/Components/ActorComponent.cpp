// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ActorComponent.h"
#include "LogGame.h"
#include "GameFramework/Actor.h"
#include "Level/World.h"

using namespace std;
using namespace std::chrono;

using enum ELogVerbosity;

void ActorComponent::ComponentTickFunction::ExecuteTick(duration<float> elapsedTime)
{
	if (_target == nullptr)
	{
		LogSystem::Log(LogTicking, Error, L"Target is nullptr.");
		return;
	}

	AActor* const owner = _target->GetOwner();
	if (owner == nullptr)
	{
		LogSystem::Log(LogTicking, Error, L"Target have not any owner actor.");
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

void ActorComponent::TickComponent(duration<float> elapsedTime, ComponentTickFunction* tickFunction)
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