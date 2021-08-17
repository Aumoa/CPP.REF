// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Level/Level.h"
#include "Level/World.h"
#include "LogGame.h"
#include "GameFramework/PlayerController.h"
#include "Info/GameMode.h"

Level::Level() : Super()
	, GameModeClass(SubclassOf<AGameMode>::StaticClass())
{
}

Level::~Level()
{
}

bool Level::LoadLevel(World* world)
{
	if (!GameModeClass.IsValid())
	{
		SE_LOG(LogWorld, Error, L"GameModeClass does not specified. Abort.");
		return false;
	}

	_gameMode = world->SpawnActor(GameModeClass);
	_playerController = world->SpawnActor(_gameMode->PlayerControllerClass);
	_playerController->SpawnCameraManager(world);

	return true;
}