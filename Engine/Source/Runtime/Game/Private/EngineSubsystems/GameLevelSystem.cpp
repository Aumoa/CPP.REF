// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameLevelSystem.h"
#include "Level/World.h"
#include "Level/Level.h"
#include "Level/ActorIterator.h"

GENERATE_BODY(SGameLevelSystem);
DEFINE_LOG_CATEGORY(LogLevel);

SGameLevelSystem::SGameLevelSystem() : Super()
{
}

SGameLevelSystem::~SGameLevelSystem()
{
}

void SGameLevelSystem::PostInit()
{
	SpawnWorld(EWorldType::GameWorld);
}

void SGameLevelSystem::Deinit()
{
	for (auto Actor : ActorIterator(_GameWorld))
	{
		Actor->DestroyActor();
	}

	_GameWorld->DestroyWorld();
	GC.SuppressFinalize(_GameWorld);

	Super::Deinit();
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

		_GameWorld = gcnew SWorld(InWorldType);
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
	return _GameWorld->OpenLevel(InLevelToOpen) != nullptr;
}