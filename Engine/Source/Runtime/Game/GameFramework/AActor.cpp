// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;
import SC.Runtime.Game;

using namespace std;
using namespace std::chrono;

using enum ELogVerbosity;

void AActor::ActorTickFunction::ExecuteTick(duration<float> elapsedTime)
{
	if (_target == nullptr)
	{
		LogSystem::Log(LogTicking, Error, L"Target is nullptr.");
		return;
	}

	if (_target->HasBegunPlay() && _target->IsActive())
	{
		_target->TickActor(elapsedTime, this);
	}
}

AActor::AActor() : Super()
	, PrimaryActorTick(this)
{
}

void AActor::TickActor(duration<float> elapsedTime, ActorTickFunction* tickFunction)
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

vector<ActorComponent*> AActor::GetOwnedComponents() const
{
	size_t cnt = 0;
	for (auto& subset : _components)
	{
		cnt += subset.second.size();
	}

	vector<ActorComponent*> cps(cnt);
	for (auto& subset : _components)
	{
		cps.insert(cps.end(), subset.second.begin(), subset.second.end());
	}
	
	return cps;
}

ActorComponent* AActor::GetComponentByClass(const type_info& type) const
{
	// Find component from actor components.
	if (auto it = _components.find(type.hash_code()); it != _components.end())
	{
		return (*it->second.begin());
	}

	// Else, find component from scene components.
	SceneComponent* item = nullptr;
	ForEachSceneComponents([&item, &type](SceneComponent* component)
	{
		if (typeid(*component).hash_code() == type.hash_code())
		{
			item = component;
			return true;
		}

		return false;
	});

	return nullptr;
}

void AActor::SetRootComponent(SceneComponent* scene)
{
	if (scene == nullptr)
	{
		LogSystem::Log(LogComponent, Error, L"The root component could not be nullptr.");
		return;
	}

	if (_rootComponent == nullptr)
	{
		LogSystem::Log(LogComponent, Verbose, L"The root component is not empty. Instance will be dangling.");
	}

	_rootComponent = scene;
	Object* outer = scene->GetOuter();
	if (outer != this)
	{
		scene->SetOuter(outer);
	}
}

SceneComponent* AActor::GetRootComponent() const
{
	return _rootComponent;
}

void AActor::ForEachSceneComponents(function<bool(SceneComponent*)> body) const
{
	if (_rootComponent == nullptr)
	{
		return;
	}

	queue<SceneComponent*> hierarchy;
	hierarchy.emplace(_rootComponent);

	while (!hierarchy.empty())
	{
		SceneComponent* top = hierarchy.front();
		if (body(top))
		{
			break;
		}

		vector<SceneComponent*> childs = top->GetChildComponents();
		for (auto& child : childs)
		{
			hierarchy.emplace(child);
		}

		hierarchy.pop();
	}
}