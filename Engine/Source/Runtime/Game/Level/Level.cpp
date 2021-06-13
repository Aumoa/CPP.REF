// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Core;
import SC.Runtime.Game;

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