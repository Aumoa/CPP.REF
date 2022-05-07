// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameLevelSystem.h"
#include "Level/World.h"
#include "Level/Level.h"
#include "Level/ActorIterator.h"

using namespace ::libty;

DEFINE_LOG_CATEGORY(LogLevel);

SGameLevelSystem::SGameLevelSystem() : Super()
{
}

SGameLevelSystem::~SGameLevelSystem()
{
}

Task<> SGameLevelSystem::StartAsync(std::stop_token CancellationToken)
{
	SpawnWorld(EWorldType::GameWorld);
	return Super::StartAsync(CancellationToken);
}

Task<> SGameLevelSystem::StopAsync(std::stop_token CancellationToken)
{
	for (auto Actor : ActorIterator(_GameWorld))
	{
		Actor->DestroyActor();
	}

	_GameWorld->DestroyWorld();
	GC->SuppressFinalize(_GameWorld);

	return Super::StopAsync(CancellationToken);
}

SWorld* SGameLevelSystem::SpawnWorld(EWorldType InWorldType)
{
	if (InWorldType == EWorldType::GameWorld)
	{
		if (_GameWorld)
		{
			throw FatalException("GameWorld cannot be multiple instantiated.");
			return nullptr;
		}

		_GameWorld = gcnew SWorld(InWorldType);
		_GameWorld->InitWorld();
		return _GameWorld;
	}
	else
	{
		throw FatalException("NOT IMPLEMENTED.");
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