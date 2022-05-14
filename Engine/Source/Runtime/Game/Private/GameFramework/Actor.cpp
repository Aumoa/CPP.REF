// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameFramework/Actor.h"
#include "LogGame.h"
#include "Level/World.h"
#include "Components/PrimitiveComponent.h"

using namespace ::libty;

DEFINE_LOG_CATEGORY(LogActor);

AActor::ActorTickFunction::ActorTickFunction(AActor* InTarget)
	: Target(InTarget)
{
}

AActor* AActor::ActorTickFunction::GetTarget() const
{
	return Target;
}

void AActor::ActorTickFunction::ExecuteTick(float InDeltaTime)
{
	if (Target->HasBegunPlay() && Target->IsActive())
	{
		Target->TickActor(InDeltaTime, this);
	}
}

AActor::AActor() : Super()
	, PrimaryActorTick(this)
	, bActive(true)
	, bHasBegunPlay(false)
{
}

void AActor::TickActor(float InDeltaTime, ActorTickFunction* ActorTickFunction)
{
	if (ActorTickFunction == &PrimaryActorTick)
	{
		Tick(InDeltaTime);
	}
}

void AActor::BeginPlay()
{
	bHasBegunPlay = true;
}

void AActor::EndPlay()
{
	bHasBegunPlay = false;
}

void AActor::Tick(float InDeltaTime)
{
}

void AActor::SetActive(bool bActive)
{
	if (bActive != bActive)
	{
		bActive = bActive;
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

void AActor::DestroyActor()
{
	SWorld* const World = GetWorld();
	if (ensureMsgf(World != nullptr, TEXT("Actor does not spawned at world.")))
	{
		World->DestroyActor(this);
	}
}

void AActor::PostInitializedComponents()
{
}

void AActor::DispatchBeginPlay()
{
	if (!HasBegunPlay())
	{
		BeginPlay();
	}

	for (auto ActorComponent : OwnedComponents)
	{
		ActorComponent->DispatchBeginPlay();
	}

	for (auto SceneComponent : GetSceneComponents())
	{
		SceneComponent->DispatchBeginPlay();
	}
}

void AActor::DispatchEndPlay()
{
	for (auto SceneComponent : GetSceneComponents())
	{
		SceneComponent->DispatchEndPlay();
	}

	for (auto ActorComponent : OwnedComponents)
	{
		ActorComponent->DispatchEndPlay();
	}

	if (HasBegunPlay())
	{
		EndPlay();
	}
}

void AActor::AddOwnedComponent(SActorComponent* InComponent)
{
	if (InComponent->GetType()->IsDerivedFrom<SSceneComponent>())
	{
		SE_LOG(LogActor, Error, TEXT("SceneComponent could not add to inline components array. Attach to root component of this actor for your desired do."));
		return;
	}

	OwnedComponents.emplace(InComponent);
	InComponent->SetOuter(this);
	InComponent->MarkOwner();
}

const std::set<SActorComponent*>& AActor::GetOwnedComponents()
{
	return OwnedComponents;
}

SActorComponent* AActor::GetComponentByClass(SubclassOf<SActorComponent> InComponentClass)
{
	if (InComponentClass->IsDerivedFrom<SSceneComponent>())
	{
		std::vector<SSceneComponent*> InlineComponentArray;
		InlineComponentArray.reserve(16);
		InlineComponentArray.emplace_back(RootComponent);

		while (InlineComponentArray.size())
		{
			SSceneComponent* Back = InlineComponentArray.back();
			InlineComponentArray.erase(InlineComponentArray.end() - 1);

			if (Back->GetType()->IsDerivedFrom(InComponentClass))
			{
				return Back;
			}
			else
			{
				const auto& ChildComponentArray = Back->GetChildComponents();
				InlineComponentArray.insert(InlineComponentArray.end(), ChildComponentArray.begin(), ChildComponentArray.end());
			}
		}

		return nullptr;
	}
	else  // InComponentClass is derived from SActorComponent.
	{
		for (auto& Component : OwnedComponents)
		{
			if (Component->GetType()->IsDerivedFrom(InComponentClass))
			{
				return Component;
			}
		}

		return nullptr;
	}
}

void AActor::SetRootComponent(SSceneComponent* InRootComponent)
{
	SSceneComponent* AttachParent = nullptr;
	String SocketName;

	if (RootComponent)
	{
		AttachParent = RootComponent->GetAttachParent();
		SocketName = RootComponent->GetAttachSocketName();

		// RootComponent will be destroyed.
		if (AttachParent)
		{
			RootComponent->DetachFromComponent();
		}

		RootComponent = nullptr;
	}

	RootComponent = InRootComponent;
	RootComponent->SetOuter(this);
	RootComponent->MarkOwner();

	if (AttachParent)
	{
		if ((size_t)SocketName == 0)
		{
			RootComponent->AttachToComponent(AttachParent);
		}
		else
		{
			RootComponent->AttachToSocket(AttachParent, SocketName);
		}
	}
}

SSceneComponent* AActor::GetRootComponent()
{
	return RootComponent;
}

std::vector<SSceneComponent*> AActor::GetSceneComponents()
{
	if (RootComponent == nullptr)
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
	if (RootComponent == nullptr)
	{
		return;
	}

	RootComponent->SetLocation(location);
}

Vector3 AActor::GetActorLocation() const
{
	if (RootComponent == nullptr)
	{
		return Vector3();
	}

	return RootComponent->GetLocation();
}

void AActor::SetActorRotation(const Quaternion& rotation)
{
	if (RootComponent == nullptr)
	{
		return;
	}

	RootComponent->SetRotation(rotation);
}

Quaternion AActor::GetActorRotation() const
{
	if (RootComponent == nullptr)
	{
		return Quaternion();
	}

	return RootComponent->GetRotation();
}