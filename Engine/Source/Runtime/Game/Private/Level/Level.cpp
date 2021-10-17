// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Level/Level.h"
#include "Level/World.h"
#include "LogGame.h"
#include "GameFramework/PlayerController.h"
#include "Info/GameMode.h"

SLevel::SLevel() : Super()
	, GameModeClass(AGameMode::StaticClass())
{
}

SLevel::~SLevel()
{
}

bool SLevel::LoadLevel(SWorld* InWorld)
{
	checkf(GameModeClass.IsValid(), L"GameModeClass does not specified.");

	_World = InWorld;

	_GameMode = InWorld->SpawnActor(GameModeClass);
	_PlayerController = _GameMode->SpawnPlayerController();

	return true;
}

void SLevel::UnloadLevel()
{
	if (_PlayerController)
	{
		_PlayerController->DestroyActor();
		_PlayerController = nullptr;
	}

	if (_GameMode)
	{
		_GameMode->DestroyActor();
		_GameMode = nullptr;
	}
}

APlayerController* SLevel::GetPlayerController()
{
	return _PlayerController;
}

SWorld* SLevel::GetWorld()
{
	return _World;
}