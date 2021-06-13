// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Core;
import SC.Runtime.Game;
import std.core;

using enum ELogVerbosity;

using namespace std;
using namespace std::chrono;

World::World(GameEngine* engine) : Super()
	, _engine(engine)
{
	SetWorld(this);
	_scene = CreateSubobject<Scene>(this, _engine->GetRHIDevice());
}

bool World::LoadLevel(SubclassOf<Level> levelToLoad)
{
	if (!levelToLoad.IsValid())
	{
		LogSystem::Log(LogWorld, Error, L"The parameter that specified class of desired to load level is nullptr. Abort.");
		return false;
	}

	// Clear spawned actors.
	for (auto& actor : _actors)
	{
		DestroySubobject(actor);
	}
	_actors.clear();

	Level* levelInstance = levelToLoad.Instantiate(this);
	if (!levelInstance->LoadLevel(this))
	{
		LogSystem::Log(LogWorld, Fatal, L"Could not load level.");
		return false;
	}

	return true;
}

void World::RegisterTickFunction(TickFunction* function)
{
	_tickInstances.emplace(function);
}

void World::RegisterComponent(ActorComponent* component)
{
	if (auto* isPrimitive = dynamic_cast<PrimitiveComponent*>(component); isPrimitive != nullptr)
	{
		PrimitiveSceneProxy* proxy = isPrimitive->CreateSceneProxy();
		if (proxy != nullptr)
		{
			int64 id = _scene->AddPrimitive(proxy);
			proxy->PrimitiveId = id;
		}
		isPrimitive->SceneProxy = proxy;
	}
}

void World::LevelTick(duration<float> elapsedTime)
{
	for (auto& func : _tickInstances)
	{
		func->Ready();
	}

	for (auto& func : _tickInstances)
	{
		func->ExecuteTick(elapsedTime);
	}
}

bool World::InternalSpawnActor(AActor* instance)
{
	// Register all actor components.
	vector<ActorComponent*> actorComponents = instance->GetOwnedComponents();
	for (auto& ac : actorComponents)
	{
		ac->RegisterComponentWithWorld(this);
	}

	// Register all scene components.
	instance->ForEachSceneComponents<SceneComponent>([this](SceneComponent* component)
	{
		component->RegisterComponentWithWorld(this);
		return false;
	});

	return _actors.emplace(instance).second;
}