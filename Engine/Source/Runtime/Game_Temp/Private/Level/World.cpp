// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Level/World.h"
#include "Level/Level.h"
#include "GameEngine.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Ticking/TickTaskLevelManager.h"

DEFINE_LOG_CATEGORY(LogWorld);

DECLARE_STAT_GROUP("World", STATGROUP_World);
DECLARE_CYCLE_STAT("LevelTick", STAT_LevelTick, STATGROUP_World);
DECLARE_CYCLE_STAT("  BeginFrame", STAT_BeginFrame, STATGROUP_World);
DECLARE_CYCLE_STAT("  PrePhysics", STAT_PrePhysics, STATGROUP_World);
DECLARE_CYCLE_STAT("  DuringPhysics", STAT_DuringPhysics, STATGROUP_World);
DECLARE_CYCLE_STAT("  PostPhysics", STAT_PostPhysics, STATGROUP_World);
DECLARE_CYCLE_STAT("  PostUpdateWork", STAT_PostUpdateWork, STATGROUP_World);

using namespace ::libty;

SWorld::SWorld(EWorldType InWorldType) : Super()
	, WorldType(InWorldType)
{
}

void SWorld::InitWorld()
{
}

void SWorld::DestroyWorld()
{
}

SWorld* SWorld::GetWorld()
{
	return this;
}

EWorldType SWorld::GetWorldType()
{
	return WorldType;
}

SLocalPlayer* SWorld::GetLocalPlayer()
{
	return nullptr;
}

SLevel* SWorld::OpenLevel(SubclassOf<SLevel> InLevelToOpen)
{
	if (!InLevelToOpen.IsValid())
	{
		SE_LOG(LogWorld, Error, TEXT("Class of level to open is not specified. Abort."));
		return nullptr;
	}

	if (Level)
	{
		Level->UnloadLevel();
	}

	Level = Cast<SLevel>(InLevelToOpen->Instantiate());
	if (!Level->LoadLevel(this))
	{
		throw Exception(TEXT("Could not load level."));
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
	SCOPE_CYCLE_COUNTER(STAT_LevelTick);

	Level->IncrementalActorsApply();
	if (STickTaskLevelManager* LevelTick = Level->GetLevelTick())
	{
		{
			SCOPE_CYCLE_COUNTER(STAT_BeginFrame);
			LevelTick->BeginFrame();
		}

		{
			SCOPE_CYCLE_COUNTER(STAT_PrePhysics);
			LevelTick->IncrementalDispatchTick(ETickingGroup::PrePhysics, InDeltaTime);
		}

		{
			SCOPE_CYCLE_COUNTER(STAT_DuringPhysics);
			LevelTick->IncrementalDispatchTick(ETickingGroup::DuringPhysics, InDeltaTime);
		}

		{
			SCOPE_CYCLE_COUNTER(STAT_PostPhysics);
			LevelTick->IncrementalDispatchTick(ETickingGroup::PostPhysics, InDeltaTime);
		}

		if (APlayerController* PlayerController = Level->GetPlayerController())
		{
			PlayerController->UpdateCameraManager(InDeltaTime);
		}

		{
			SCOPE_CYCLE_COUNTER(STAT_PostUpdateWork);
			LevelTick->IncrementalDispatchTick(ETickingGroup::PostUpdateWork, InDeltaTime);
		}

		LevelTick->EndFrame();
	}
}