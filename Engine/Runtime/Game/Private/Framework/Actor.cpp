// Copyright 2020 Aumoa.lib. All right reserved.

#include "Framework/Actor.h"

#include "Components/SceneComponent.h"
#include "Logging/LogMacros.h"

using namespace std;
using namespace std::chrono;

AActor::ActorTickFunction::ActorTickFunction()
	: Target(nullptr)
{

}

AActor::ActorTickFunction::~ActorTickFunction()
{

}

void AActor::ActorTickFunction::ExecuteTick(duration<double> deltaTime)
{
	if (Target == nullptr)
	{
		SE_LOG(LogTicking, Error, L"Target is nullptr.");
		return;
	}

	Target->Tick(deltaTime);
}

AActor::AActor() : Super()
{
	PrimaryActorTick.Target = this;
}

AActor::~AActor()
{

}

void AActor::BeginPlay()
{

}

void AActor::EndPlay()
{

}

void AActor::Tick(duration<double> deltaTime)
{

}

SceneComponent* AActor::RootComponent_get() const
{
	return rootComponent.Get();
}

void AActor::RootComponent_set(SceneComponent* value)
{
	rootComponent = value;
}

auto AActor::PrimaryActorTick_get() -> ActorTickFunction&
{
	return primaryActorTick;
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