// Copyright 2020-2021 Aumoa.lib. All right reserved.

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
	_World = NewObject<SWorld>();
}

void SGameLevelSystem::Deinit()
{
	for (auto& Actor : _World->GetAllActors())
	{
		Actor->DestroyActor();
	}
}

SWorld* SGameLevelSystem::GetWorld() const
{
	return _World;
}

bool SGameLevelSystem::OpenLevel(SubclassOf<SLevel> levelToLoad)
{
	if (!levelToLoad.IsValid())
	{
		SE_LOG(LogLevel, Error, L"Could not open level.");
		return false;
	}

	SLevel* level = levelToLoad.Instantiate(this);
	if (!level->LoadLevel(_World))
	{
		SE_LOG(LogLevel, Error, L"Could not load level.");
		return false;
	}

	if (_LoadedLevel)
	{
		// TODO: Unload level.
		_LoadedLevel = nullptr;
	}

	_LoadedLevel = level;
	return true;
}

SLevel* SGameLevelSystem::GetLevel() const
{
	return _LoadedLevel;
}