// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameFramework/Actor.h"
#include "LogGame.h"
#include "Level/World.h"
#include "Components/PrimitiveComponent.h"

DEFINE_LOG_CATEGORY(LogActor);

void AActor::SActorTickFunction::ExecuteTick(float elapsedTime)
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

void AActor::TickActor(float elapsedTime, SActorTickFunction* tickFunction)
{
	if (tickFunction == &PrimaryActorTick)
	{
		Tick(elapsedTime);
	}
}

void AActor::BeginPlay()
{
	_bHasBegunPlay = true;
}

void AActor::EndPlay()
{
	_bHasBegunPlay = false;
}

void AActor::Tick(float elapsedTime)
{
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

void AActor::DestroyActor()
{
	SWorld* const World = GetWorld();
	if (ensureMsgf(World != nullptr, L"Actor does not spawned at world."))
	{
		World->DestroyActor(this);
	}
}

void AActor::PostInitializedComponents()
{
}

void AActor::AddOwnedComponent(SActorComponent* InComponent)
{
	if (InComponent->GetType()->IsDerivedFrom<SSceneComponent>())
	{
		SE_LOG(LogActor, Error, L"SceneComponent could not add to inline components array. Attach to root component of this actor for your desired do.");
		return;
	}

	_Components.emplace(InComponent);
}

const std::set<SActorComponent*>& AActor::GetOwnedComponents()
{
	return _Components;
}

SActorComponent* AActor::GetComponentByClass(SubclassOf<SActorComponent> InComponentClass)
{
	const Type* Class = InComponentClass.GetType();
	if (Class->IsDerivedFrom<SSceneComponent>())
	{
		for (auto& Component : GetSceneComponents())
		{
			if (Component->GetType()->IsDerivedFrom(Class))
			{
				return Component;
			}
		}

		return nullptr;
	}
	else  // InComponentClass is derived from SActorComponent.
	{
		for (auto& Component : _Components)
		{
			if (Component->GetType()->IsDerivedFrom(Class))
			{
				return Component;
			}
		}

		return nullptr;
	}
}

void AActor::SetRootComponent(SSceneComponent* InRootComponent)
{
	if (_RootComponent)
	{
		// RootComponent will be destroyed.
		_RootComponent->DetachFromComponent();
		if (_RootComponent->GetOuter() == this)
		{
			DestroySubobject(_RootComponent);
		}
		_RootComponent = nullptr;
	}

	_RootComponent = InRootComponent;
	_RootComponent->SetOuter(this);

	for (auto& ChildComponent : GetSceneComponents())
	{
		ChildComponent->SetOwnerPrivate(this);
	}
}

SSceneComponent* AActor::GetRootComponent()
{
	return _RootComponent;
}

std::vector<SSceneComponent*> AActor::GetSceneComponents()
{
	if (_RootComponent == nullptr)
	{
		return {};
	}

	std::vector<SSceneComponent*> ComponentsArray;
	ComponentsArray.reserve(16);
	ComponentsArray.emplace_back(GetRootComponent());
	
	std::vector<SSceneComponent*> Stack;
	Stack.reserve(16);
	Stack.emplace_back(GetRootComponent());
	while (!Stack.empty())
	{
		SSceneComponent* MyRoot = Stack.back();
		Stack.erase(--Stack.end());

		// Add stack for search as new depth.
		size_t ComponentsBegin = Stack.size();
		for (auto& ChildComponent : MyRoot->GetChildComponents())
		{
			Stack.emplace_back(ChildComponent);
		}

		// Add components to result array.
		if (ComponentsBegin != Stack.size())
		{
			ComponentsArray.assign(Stack.begin() + ComponentsBegin, Stack.end());
		}
	}

	return ComponentsArray;
}

void AActor::SetActorLocation(const Vector3& location)
{
	if (_RootComponent == nullptr)
	{
		return;
	}

	_RootComponent->SetLocation(location);
}

Vector3 AActor::GetActorLocation() const
{
	if (_RootComponent == nullptr)
	{
		return Vector3();
	}

	return _RootComponent->GetLocation();
}

void AActor::SetActorRotation(const Quaternion& rotation)
{
	if (_RootComponent == nullptr)
	{
		return;
	}

	_RootComponent->SetRotation(rotation);
}

Quaternion AActor::GetActorRotation() const
{
	if (_RootComponent == nullptr)
	{
		return Quaternion();
	}

	return _RootComponent->GetRotation();
}