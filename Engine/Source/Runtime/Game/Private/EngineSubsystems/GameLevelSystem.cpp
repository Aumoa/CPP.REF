// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EngineSubsystems/GameLevelSystem.h"
#include "Level/World.h"
#include "Level/Level.h"

DEFINE_LOG_CATEGORY(LogLevel);

SGameLevelSystem::SGameLevelSystem() : Super()
{
}

SGameLevelSystem::~SGameLevelSystem()
{
}

void SGameLevelSystem::Init()
{
	Super::Init();
	_world = NewObject<World>();
}

World* SGameLevelSystem::GetWorld() const
{
	return _world;
}

bool SGameLevelSystem::OpenLevel(SubclassOf<SLevel> levelToLoad)
{
	if (!levelToLoad.IsValid())
	{
		SE_LOG(LogLevel, Error, L"Could not open level.");
		return false;
	}

	SLevel* level = levelToLoad.Instantiate(this);
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

SLevel* SGameLevelSystem::GetLevel() const
{
	return _loadedLevel;
}