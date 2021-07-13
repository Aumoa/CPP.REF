// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Level.h"
#include "LogGame.h"
#include "World.h"
#include "GameFramework/PlayerController.h"
#include "Info/GameMode.h"

using enum ELogVerbosity;

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
		LogSystem::Log(LogWorld, Error, L"GameModeClass does not specified. Abort.");
		return false;
	}

	_gameMode = world->SpawnActor(GameModeClass);
	_playerController = world->SpawnActor(_gameMode->PlayerControllerClass);
	_playerController->SpawnCameraManager(world);

	return true;
}