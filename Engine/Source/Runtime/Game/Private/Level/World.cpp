// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Level/World.h"
#include "Level/Level.h"
#include "GameEngine.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "Scene/Scene.h"
#include "Scene/PrimitiveSceneProxy.h"
#include "Camera/PlayerCameraManager.h"
#include "EngineSubsystems/GameRenderSystem.h"

SWorld::SWorld() : Super()
{
	SetWorld(this);
	_scene = NewObject<SScene>(this, GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetRHIDevice());
}

AActor* SWorld::SpawnActor(SubclassOf<AActor> actorClass)
{
	if (!actorClass.IsValid())
	{
		LogSystem::Log(LogWorld, ELogVerbosity::Error, L"Actor class does not specified. Abort.");
		return nullptr;
	}

	AActor* spawnedActor = actorClass.Instantiate(this);
	if (!InternalSpawnActor(spawnedActor))
	{
		DestroySubobject(spawnedActor);
		return nullptr;
	}
	return spawnedActor;
}

SLevel* SWorld::LoadLevel(SubclassOf<SLevel> levelToLoad)
{
	if (!levelToLoad.IsValid())
	{
		SE_LOG(LogWorld, Error, L"The parameter that specified class of desired to load level is nullptr. Abort.");
		return nullptr;
	}

	// Clear spawned actors.
	for (auto& actor : _actors)
	{
		DestroySubobject(actor);
	}
	_actors.clear();

	SLevel* levelInstance = levelToLoad.Instantiate(this);
	if (!levelInstance->LoadLevel(this))
	{
		SE_LOG(LogWorld, Fatal, L"Could not load level.");
		DestroySubobject(levelInstance);
		return nullptr;
	}

	// Begin play for all actors.
	_level = levelInstance;
	for (auto& actor : _actors)
	{
		actor->BeginPlay();
	}

	return levelInstance;
}

void SWorld::RegisterTickFunction(STickFunction* function)
{
	if (function->bCanEverTick)
	{
		_tickInstances.emplace(function);
	}
}

void SWorld::RegisterComponent(SActorComponent* component)
{
	if (auto* isPrimitive = dynamic_cast<SPrimitiveComponent*>(component); isPrimitive != nullptr)
	{
		SPrimitiveSceneProxy* proxy = isPrimitive->CreateSceneProxy();
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

void SWorld::UnregisterTickFunction(STickFunction* function)
{
	_tickInstances.erase(function);
}

void SWorld::UnregisterComponent(SActorComponent* component)
{
	if (auto* isPrimitive = dynamic_cast<SPrimitiveComponent*>(component); isPrimitive != nullptr)
	{
		if (isPrimitive->SceneProxy != nullptr)
		{
			int64 id = isPrimitive->SceneProxy->PrimitiveId;
			_scene->RemovePrimitive(id);
		}
	}
}

void SWorld::LevelTick(std::chrono::duration<float> elapsedTime)
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

bool SWorld::InternalSpawnActor(AActor* instance)
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