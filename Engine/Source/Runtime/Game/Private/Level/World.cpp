// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Level/World.h"
#include "Level/Level.h"
#include "GameEngine.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "SceneRendering/Scene.h"
#include "Scene/PrimitiveSceneProxy.h"
#include "Camera/PlayerCameraManager.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "Ticking/TickTaskLevelManager.h"

SWorld::SWorld(EWorldType InWorldType) : Super()
	, _WorldType(InWorldType)
{
}

void SWorld::InitWorld()
{
	_Scene = NewObject<SScene>(GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetRHIDevice());
}

void SWorld::DestroyWorld()
{
	if (_Scene)
	{
		DestroySubobject(_Scene);
		_Scene = nullptr;
	}
}

SWorld* SWorld::GetWorld()
{
	return this;
}

EWorldType SWorld::GetWorldType()
{
	return _WorldType;
}

SLevel* SWorld::OpenLevel(SubclassOf<SLevel> InLevelToOpen)
{
	if (!InLevelToOpen.IsValid())
	{
		SE_LOG(LogWorld, Error, L"Class of level to open is not specified. Abort.");
		return nullptr;
	}

	if (_Level)
	{
		_Level->UnloadLevel();
		DestroySubobject(_Level);
	}

	SLevel* LevelInstance = InLevelToOpen.Instantiate(this);
	if (!LevelInstance->LoadLevel(this))
	{
		SE_LOG(LogWorld, Fatal, L"Could not load level.");
		DestroySubobject(LevelInstance);
		return nullptr;
	}

	_Level = LevelInstance;
	return LevelInstance;
}

SLevel* SWorld::GetLevel()
{
	return _Level;
}

AActor* SWorld::SpawnActor(SubclassOf<AActor> InActorClass, bool bSpawnIncremental)
{
	if (!InActorClass.IsValid())
	{
		SE_LOG(LogWorld, Error, L"Actor class does not specified. Abort.");
		return nullptr;
	}

	AActor* Actor = InActorClass.Instantiate(_Level);
	if (Actor == nullptr)
	{
		SE_LOG(LogWorld, Error, L"Actor class does not support instantiate without any constructor arguments.");
		return nullptr;
	}

	if (bSpawnIncremental)
	{
		_Level->ActorsToAdd.emplace_back(Actor);
	}
	else
	{
		_Level->InternalAddActor(Actor);
	}

	return Actor;
}

void SWorld::DestroyActor(AActor* InActor)
{
	_Level->ActorsToRemove.emplace_back(InActor);
}

void SWorld::LevelTick(float InDeltaTime)
{
	_Level->IncrementalActorsApply();
	if (STickTaskLevelManager* LevelTick = _Level->GetLevelTick())
	{
		LevelTick->BeginFrame();

		LevelTick->IncrementalDispatchTick(ETickingGroup::PrePhysics, InDeltaTime);
		LevelTick->IncrementalDispatchTick(ETickingGroup::DuringPhysics, InDeltaTime);
		LevelTick->IncrementalDispatchTick(ETickingGroup::PostPhysics, InDeltaTime);
		LevelTick->IncrementalDispatchTick(ETickingGroup::PostUpdateWork, InDeltaTime);

		LevelTick->EndFrame();
	}
}