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

DEFINE_LOG_CATEGORY(LogWorld);

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
		DestroyObject(_Scene);
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

SScene* SWorld::GetScene()
{
	return _Scene;
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
		DestroyObject(_Level);
	}

	_Level = InLevelToOpen.Instantiate(this);
	if (!_Level->LoadLevel(this))
	{
		SE_LOG(LogWorld, Fatal, L"Could not load level.");
		DestroyObject(_Level);
		return nullptr;
	}

	return _Level;
}

SLevel* SWorld::GetLevel()
{
	return _Level;
}

AActor* SWorld::SpawnActor(SubclassOf<AActor> InActorClass)
{
	return _Level->SpawnActor(InActorClass, false);
}

void SWorld::DestroyActor(AActor* InActor)
{
	_Level->DestroyActor(InActor);
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

		if (APlayerController* PlayerController = _Level->GetPlayerController())
		{
			PlayerController->UpdateCameraManager(InDeltaTime);
		}

		LevelTick->IncrementalDispatchTick(ETickingGroup::PostUpdateWork, InDeltaTime);

		LevelTick->EndFrame();
	}
}