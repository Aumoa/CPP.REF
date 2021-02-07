// Copyright 2020 Aumoa.lib. All right reserved.

#include "Framework/Actor.h"

#include "Components/SceneComponent.h"
#include "Logging/LogMacros.h"
#include "Logging/EngineLogCategory.h"

using namespace std;
using namespace std::chrono;

AActor::ActorTickFunction::ActorTickFunction() : Super()
	, Target(nullptr)
{

}

AActor::ActorTickFunction::~ActorTickFunction()
{

}

void AActor::ActorTickFunction::ExecuteTick(Seconds deltaTime)
{
	if (Target == nullptr)
	{
		SE_LOG(LogTicking, Error, L"Target is nullptr.");
		return;
	}

	if (Target->HasBegunPlay && Target->ActorTickEnabled)
	{
		Target->TickActor(deltaTime);
	}

	Super::ExecuteTick(deltaTime);
}

AActor::AActor() : Super()
	, bActorTickEnabled(true)
	, world(nullptr)
	, bActorHasBegunPlay(false)
	, rootComponent(nullptr)
{
	PrimaryActorTick.Target = this;
}

AActor::~AActor()
{

}

TickFunction* AActor::GetTickFunction()
{
	return &PrimaryActorTick;
}

void AActor::AddPrerequisiteObject(ITickFunctionObject* inObject)
{
	PrimaryActorTick.AddPrerequisite(inObject);
}

void AActor::RemovePrerequisiteObject(ITickFunctionObject* inObject)
{
	PrimaryActorTick.RemovePrerequisite(inObject);
}

void AActor::BeginPlay()
{
	bActorHasBegunPlay = true;

	for (auto& component_pair : ownedComponents)
	{
		auto& component = component_pair.first;
		if (!component->HasBegunPlay)
		{
			component->owner = this;
			component->BeginPlay();
		}
	}
}

void AActor::EndPlay()
{
	for (auto& component_pair : ownedComponents)
	{
		auto& component = component_pair.first;
		if (component->HasBegunPlay)
		{
			component->EndPlay();
			component->owner = nullptr;
		}
	}

	bActorHasBegunPlay = false;
}

void AActor::TickActor(Seconds deltaTime)
{
	Tick(deltaTime);
}

World* AActor::GetWorld() const
{
	return world;
}

void AActor::SetWorld(World* world)
{
	this->world = world;
}

#define NO_ROOT_WARNINGS(...) \
if (rootComponent == nullptr)\
{\
	SE_LOG(LogActor, Warning, L"{0} called with actor that have not a root scene component. Function will be return identity transform.", __FUNCTIONW__);\
	return __VA_ARGS__;\
}

Transform AActor::GetActorTransform() const
{
	NO_ROOT_WARNINGS(Transform::Identity);
	return rootComponent->ComponentTransform;
}

void AActor::SetActorTransform(const Transform& value)
{
	NO_ROOT_WARNINGS();

	Transform relativeTransform;
	if (rootComponent->AttachParent != nullptr)
	{
		relativeTransform = value.GetRelativeTransform(rootComponent->AttachParent->GetSocketTransform(rootComponent->AttachSocketName));
	}
	else
	{
		relativeTransform = value;
	}
	rootComponent->RelativeTransform = relativeTransform;
}

Vector3 AActor::GetActorLocation() const
{
	NO_ROOT_WARNINGS(Vector3::Zero);
	return rootComponent->ComponentLocation;
}

void AActor::SetActorLocation(const Vector3& value)
{
	NO_ROOT_WARNINGS();
	auto delta = value - GetActorLocation();
	rootComponent->MoveComponent(delta, GetActorRotation());
}

Vector3 AActor::GetActorScale() const
{
	NO_ROOT_WARNINGS(Vector3::One);
	return rootComponent->Scale;
}

void AActor::SetActorScale(const Vector3& value)
{
	NO_ROOT_WARNINGS();

	Transform relativeTransform;
	if (rootComponent->AttachParent != nullptr)
	{
		auto worldTransform = Transform(GetActorLocation(), value, GetActorRotation());
		relativeTransform = worldTransform.GetRelativeTransform(rootComponent->AttachParent->GetSocketTransform(rootComponent->AttachSocketName));
	}
	else
	{
		relativeTransform = Transform(GetActorLocation(), value, GetActorRotation());
	}
	rootComponent->RelativeTransform = relativeTransform;
}

Quaternion AActor::GetActorRotation() const
{
	NO_ROOT_WARNINGS(Quaternion::Identity);
	return rootComponent->ComponentRotation;
}

void AActor::SetActorRotation(const Quaternion& value)
{
	NO_ROOT_WARNINGS();
	rootComponent->MoveComponent(Vector3::Zero, value);
}

#undef NO_ROOT_WARNINGS

SceneComponent* AActor::RootComponent_get() const
{
	return rootComponent;
}

void AActor::RootComponent_set(SceneComponent* value)
{
	rootComponent = value;
}

auto AActor::PrimaryActorTick_get() -> ActorTickFunction&
{
	return primaryActorTick;
}

bool AActor::ActorTickEnabled_get() const
{
	return bActorTickEnabled;
}

void AActor::ActorTickEnabled_set(bool value)
{
	bActorTickEnabled = value;
}

bool AActor::HasBegunPlay_get() const
{
	return bActorHasBegunPlay;
}

void AActor::Tick(Seconds deltaTime)
{

}

bool AActor::AddComponentInternal(TRefPtr<ActorComponent>&& assign_ptr, const size_t* hierarchy, size_t num)
{
	ActorComponent* ptr = assign_ptr.Get();
	if (ptr == nullptr)
	{
		return false;
	}

	// Add to hierarchy map.
	for (size_t i = 0; i < num; ++i)
	{
		auto it = this->hierarchy.find(hierarchy[i]);
		if (it == this->hierarchy.end())
		{
			it = this->hierarchy.emplace(hierarchy[i], list<ActorComponent*>()).first;
		}
		it->second.push_back(ptr);
	}

	if (HasBegunPlay)
	{
		assign_ptr->owner = this;
		assign_ptr->BeginPlay();
	}
	ComponentAdded.Invoke(assign_ptr.Get());

	// Add to owned list.
	return ownedComponents.emplace(ptr, move(assign_ptr)).second;
}

bool AActor::RemoveComponentInternal(size_t hash_code)
{
	auto it = hierarchy.find(hash_code);
	if (it == hierarchy.end())
	{
		return false;
	}

	vector<size_t> remove_list;
	remove_list.reserve(hierarchy.size());

	auto front = it->second.front();
	for (auto& item : hierarchy)
	{
		auto erase_it = find(item.second.begin(), item.second.end(), front);
		if (erase_it != item.second.end())
		{
			ActorComponent* component = (*erase_it);
			if (component->HasBegunPlay)
			{
				component->EndPlay();
				component->owner = nullptr;
			}
			ComponentRemoved.Invoke(component);

			item.second.erase(erase_it);
			if (item.second.empty())
			{
				remove_list.emplace_back(item.first);
			}
		}
	}

	for (auto& item : remove_list)
	{
		hierarchy.erase(item);
	}

	ownedComponents.erase(front);
	return true;
}