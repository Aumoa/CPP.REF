// Copyright 2020-2021 Aumoa.lib. All right reserved.

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
	APlayerController* localPlayer = world->SpawnActor(_gameMode->PlayerControllerClass);
	localPlayer->SpawnCameraManager(world);

	return true;
}