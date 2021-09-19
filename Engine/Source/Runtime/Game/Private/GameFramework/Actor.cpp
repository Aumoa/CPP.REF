// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameFramework/Actor.h"
#include "LogGame.h"
#include "Level/World.h"
#include "Components/PrimitiveComponent.h"

void AActor::ActorTickFunction::ExecuteTick(std::chrono::duration<float> elapsedTime)
{
	if (_target == nullptr)
	{
		SE_LOG(LogTicking, Error, L"Target is nullptr.");
		return;
	}

	if (_target->HasBegunPlay() && _target->IsActive())
	{
		_target->TickActor(elapsedTime, this);
	}
}

AActor::AActor() : Super()
	, PrimaryActorTick(this)
	, _bActive(true)
	, _bHasBegunPlay(false)
{
}

void AActor::TickActor(std::chrono::duration<float> elapsedTime, ActorTickFunction* tickFunction)
{
}

void AActor::BeginPlay()
{
	_bHasBegunPlay = true;

	for (auto& component : _components)
	{
		component->BeginPlay();
	}

	// Register all scene components.
	ForEachSceneComponents<SSceneComponent>([](SSceneComponent* component)
	{
		component->BeginPlay();
		return false;
	});
}

void AActor::EndPlay()
{
	_bHasBegunPlay = false;
}

void AActor::SetActive(bool bActive)
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

void AActor::RegisterActorWithWorld(SWorld* world)
{
	world->RegisterTickFunction(&PrimaryActorTick);

	for (auto& component : _components)
	{
		component->RegisterComponentWithWorld(world);
	}

	// Register all scene components.
	ForEachSceneComponents<SSceneComponent>([world](SSceneComponent* component)
	{
		component->RegisterComponentWithWorld(world);
		return false;
	});
}

void AActor::DestroyActor()
{
	SWorld* const world = GetWorld();
	ensureMsgf(world != nullptr, L"Actor does not spawned at world.");
	
	for (auto& component : _components)
	{
		component->UnregisterComponent();
	}

	// Register all scene components.
	ForEachSceneComponents<SSceneComponent>([world](SSceneComponent* component)
	{
		component->UnregisterComponent();
		return false;
	});
}

void AActor::AddOwnedComponent(SActorComponent* component)
{
	_components.emplace(component);
}

std::set<SActorComponent*> AActor::GetOwnedComponents() const
{
	return _components;
}

void AActor::SetRootComponent(SSceneComponent* scene)
{
	if (scene == nullptr)
	{
		SE_LOG(LogComponent, Error, L"The root component could not be nullptr.");
		return;
	}

	if (_rootComponent != nullptr)
	{
		SE_LOG(LogComponent, Warning, L"The root component is not empty. Instance will be dangling.");
	}

	_rootComponent = scene;
	SObject* outer = scene->GetOuter();
	if (outer != this)
	{
		scene->SetOuter(outer);
	}

	ForEachSceneComponents<SSceneComponent>([this](SSceneComponent* ptr)
	{
		ptr->SetOwnerPrivate(this);
		return false;
	});
}

SSceneComponent* AActor::GetRootComponent() const
{
	return _rootComponent;
}

void AActor::SetActorLocation(const Vector3& location)
{
	if (_rootComponent == nullptr)
	{
		return;
	}

	_rootComponent->SetLocation(location);
}

Vector3 AActor::GetActorLocation() const
{
	if (_rootComponent == nullptr)
	{
		return Vector3();
	}

	return _rootComponent->GetLocation();
}

void AActor::SetActorRotation(const Quaternion& rotation)
{
	if (_rootComponent == nullptr)
	{
		return;
	}

	_rootComponent->SetRotation(rotation);
}

Quaternion AActor::GetActorRotation() const
{
	if (_rootComponent == nullptr)
	{
		return Quaternion();
	}

	return _rootComponent->GetRotation();
}