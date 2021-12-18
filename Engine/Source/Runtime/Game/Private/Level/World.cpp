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
#include "Application/SlateApplication.h"

GENERATE_BODY(SWorld);
DEFINE_LOG_CATEGORY(LogWorld);

SWorld::SWorld(EWorldType InWorldType) : Super()
	, WorldType(InWorldType)
{
}

void SWorld::InitWorld()
{
	Scene = NewObject<SScene>(GEngine->GetEngineSubsystem<SGameRenderSystem>()->GetRHIDevice());
}

void SWorld::DestroyWorld()
{
	if (Scene)
	{
		Scene->Dispose();
		Scene = nullptr;
	}
}

SWorld* SWorld::GetWorld()
{
	return this;
}

EWorldType SWorld::GetWorldType()
{
	return WorldType;
}

SScene* SWorld::GetScene()
{
	return Scene;
}

SLocalPlayer* SWorld::GetLocalPlayer()
{
	return GEngine->GetSlateApplication()->GetLocalPlayer();
}

SLevel* SWorld::OpenLevel(SubclassOf<SLevel> InLevelToOpen)
{
	if (!InLevelToOpen.IsValid())
	{
		SE_LOG(LogWorld, Error, L"Class of level to open is not specified. Abort.");
		return nullptr;
	}

	if (Level)
	{
		Level->UnloadLevel();
	}

	Level = Cast<SLevel>(InLevelToOpen->Instantiate());
	if (!Level->LoadLevel(this))
	{
		SE_LOG(LogWorld, Fatal, L"Could not load level.");
		return nullptr;
	}

	return Level;
}

SLevel* SWorld::GetLevel()
{
	return Level;
}

AActor* SWorld::SpawnActor(SubclassOf<AActor> InActorClass)
{
	return Level->SpawnActor(InActorClass, false);
}

void SWorld::DestroyActor(AActor* InActor)
{
	Level->DestroyActor(InActor);
}

void SWorld::LevelTick(float InDeltaTime)
{
	Level->IncrementalActorsApply();
	if (STickTaskLevelManager* LevelTick = Level->GetLevelTick())
	{
		LevelTick->BeginFrame();

		LevelTick->IncrementalDispatchTick(ETickingGroup::PrePhysics, InDeltaTime);
		LevelTick->IncrementalDispatchTick(ETickingGroup::DuringPhysics, InDeltaTime);
		LevelTick->IncrementalDispatchTick(ETickingGroup::PostPhysics, InDeltaTime);

		if (APlayerController* PlayerController = Level->GetPlayerController())
		{
			PlayerController->UpdateCameraManager(InDeltaTime);
		}

		LevelTick->IncrementalDispatchTick(ETickingGroup::PostUpdateWork, InDeltaTime);

		LevelTick->EndFrame();
	}
}