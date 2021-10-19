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
}

void SGameLevelSystem::Deinit()
{
	for (auto& Actor : _GameWorld->GetAllActors())
	{
		Actor->DestroyActor();
	}

	_GameWorld->DestroyWorld();
}

SWorld* SGameLevelSystem::SpawnWorld(EWorldType InWorldType)
{
	if (InWorldType == EWorldType::GameWorld)
	{
		if (_GameWorld)
		{
			SE_LOG(LogLevel, Fatal, L"GameWorld cannot be multiple instantiated.");
			return nullptr;
		}

		_GameWorld = NewObject<SWorld>(InWorldType);
		_GameWorld->InitWorld();
		return _GameWorld;
	}
	else
	{
		SE_LOG(LogLevel, Fatal, L"NOT IMPLEMENTED.");
		return nullptr;
	}
}

SWorld* SGameLevelSystem::GetGameWorld() const
{
	return _GameWorld;
}

bool SGameLevelSystem::OpenLevel(SubclassOf<SLevel> InLevelToOpen)
{
	_GameWorld->OpenLevel(InLevelToOpen);
}