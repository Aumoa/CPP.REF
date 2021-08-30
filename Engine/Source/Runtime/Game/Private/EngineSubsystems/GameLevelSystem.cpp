// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EngineSubsystems/GameLevelSystem.h"
#include "Level/World.h"
#include "Level/Level.h"

DEFINE_LOG_CATEGORY(LogLevel);

GameLevelSystem::GameLevelSystem() : Super()
{
}

GameLevelSystem::~GameLevelSystem()
{
}

void GameLevelSystem::Init()
{
	Super::Init();
	_world = NewObject<World>();
}

World* GameLevelSystem::GetWorld() const
{
	return _world;
}

bool GameLevelSystem::OpenLevel(SubclassOf<Level> levelToLoad)
{
	if (!levelToLoad.IsValid())
	{
		SE_LOG(LogLevel, Error, L"Could not open level.");
		return false;
	}

	Level* level = levelToLoad.Instantiate(this);
	if (!level->LoadLevel(_world))
	{
		SE_LOG(LogLevel, Error, L"Could not load level.");
		return false;
	}

	if (_loadedLevel)
	{
		// TODO: Unload level.
		_loadedLevel = nullptr;
	}

	_loadedLevel = level;
	return true;
}

Level* GameLevelSystem::GetLevel() const
{
	return _loadedLevel;
}