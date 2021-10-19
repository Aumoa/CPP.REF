// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Level/Level.h"
#include "Level/World.h"
#include "LogGame.h"
#include "GameFramework/PlayerController.h"
#include "Info/GameMode.h"
#include "Ticking/TickTaskLevelManager.h"

SLevel::SLevel() : Super()
	, GameModeClass(AGameMode::StaticClass())
{
}

SLevel::~SLevel()
{
}

bool SLevel::LoadLevel(SWorld* InWorld, STickTaskLevelManager* InParentLevelTick)
{
	checkf(GameModeClass.IsValid(), L"GameModeClass does not specified.");

	_World = InWorld;

	_GameMode = InWorld->SpawnActor(GameModeClass);
	_PlayerController = _GameMode->SpawnPlayerController();

	_LevelTick = InParentLevelTick;
	if (_LevelTick == nullptr)
	{
		_LevelTick = NewObject<STickTaskLevelManager>();
	}

	return true;
}

void SLevel::UnloadLevel()
{
	IncrementalActorsApply(0);
}

void SLevel::IncrementalActorsApply(size_t InLimit)
{
	if (InLimit == 0)
	{
		InLimit = std::numeric_limits<size_t>::max();
	}

	// Remove actors first.
	auto EndIt = ActorsToRemove.end();
	for (auto It = ActorsToRemove.begin(); InLimit-- && It != ActorsToRemove.end(); ++It)
	{
		InternalRemoveActor(*It);
		std::swap(*It, *EndIt--);
	}
	ActorsToRemove.erase(EndIt, ActorsToRemove.end());

	EndIt = ActorsToAdd.end();
	for (auto It = ActorsToAdd.begin(); InLimit-- && It != ActorsToAdd.end(); ++It)
	{
		InternalAddActor(*It);
		std::swap(*It, *EndIt--);
	}
	ActorsToAdd.erase(EndIt, ActorsToAdd.end());
}

APlayerController* SLevel::GetPlayerController()
{
	return _PlayerController;
}

SWorld* SLevel::GetWorld()
{
	return _World;
}

STickTaskLevelManager* SLevel::GetLevelTick()
{
	return _LevelTick;
}

void SLevel::InternalRemoveActor(AActor* InActor)
{
	// Unregister Actor!!
	check(false);
}

void SLevel::InternalAddActor(AActor* InActor)
{
	// Register Actor!!
	check(false);
}