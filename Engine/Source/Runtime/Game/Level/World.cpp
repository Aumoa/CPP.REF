// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "World.h"
#include "GameEngine.h"
#include "Level.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "Scene/Scene.h"
#include "Scene/PrimitiveSceneProxy.h"
#include "Camera/PlayerCameraManager.h"

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

	// Begin play for all actors.
	_level = levelInstance;
	for (auto& actor : _actors)
	{
		actor->BeginPlay();
	}

	return true;
}

void World::RegisterTickFunction(TickFunction* function)
{
	if (function->bCanEverTick)
	{
		_tickInstances.emplace(function);
	}
}

void World::RegisterComponent(ActorComponent* component)
{
	if (auto* isPrimitive = dynamic_cast<PrimitiveComponent*>(component); isPrimitive != nullptr)
	{
		PrimitiveSceneProxy* proxy = isPrimitive->CreateSceneProxy();
		if (isPrimitive->SceneProxy != nullptr)
		{
			// Actually need to remove previous scene proxy from scene.
			_scene->RemovePrimitive(isPrimitive->SceneProxy->PrimitiveId);
			DestroySubobject(isPrimitive->SceneProxy);
		}

		if (proxy != nullptr)
		{
			int64 id = _scene->AddPrimitive(proxy);
			proxy->PrimitiveId = id;
		}

		isPrimitive->SceneProxy = proxy;
	}
}

void World::UnregisterTickFunction(TickFunction* function)
{
	_tickInstances.erase(function);
}

void World::UnregisterComponent(ActorComponent* component)
{
	if (auto* isPrimitive = dynamic_cast<PrimitiveComponent*>(component); isPrimitive != nullptr)
	{
		if (isPrimitive->SceneProxy != nullptr)
		{
			int64 id = isPrimitive->SceneProxy->PrimitiveId;
			_scene->RemovePrimitive(id);
		}
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

	_playerController->UpdateCameraManager(elapsedTime);

	_scene->UpdateScene(elapsedTime);
}

bool World::InternalSpawnActor(AActor* instance)
{
	// Register.
	instance->RegisterActorWithWorld(this);

	if (auto* playerController = dynamic_cast<APlayerController*>(instance); playerController != nullptr)
	{
		_playerController = playerController;
	}

	if (auto* playerCamera = dynamic_cast<APlayerCameraManager*>(instance); playerCamera != nullptr)
	{
		_playerCamera = playerCamera;
	}

	return _actors.emplace(instance).second;
}