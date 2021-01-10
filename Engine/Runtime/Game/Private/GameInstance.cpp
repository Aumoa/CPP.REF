// Copyright 2020 Aumoa.lib. All right reserved.

#include "GameInstance.h"

#include "World.h"
#include "Framework/GameModeBase.h"
#include "Framework/PlayerController.h"
#include "Logging/LogMacros.h"
#include "Logging/EngineLogCategory.h"

using namespace std;
using namespace std::chrono;

TRefPtr<String> GameInstance::defaultAppName = L"GameInstance";

GameInstance::GameInstance() : Super()
	, gameMode(nullptr)
	, localPlayerController(nullptr)
{
	GameModeClass = TSubclassOf<AGameModeBase>::StaticClass();
}

GameInstance::~GameInstance()
{

}

TRefPtr<String> GameInstance::ToString() const
{
	return defaultAppName;
}

void GameInstance::Initialize()
{
	world = NewObject<World>();
	gameMode = world->SpawnActor(GameModeClass);
	localPlayerController = world->SpawnActor(gameMode->PlayerControllerClass);
	if (localPlayerController == nullptr)
	{
		SE_LOG(LogEngine, Fatal, L"Cannot create player controller.");
		return;
	}
	world->LoadLevel(gameMode->StartLevelClass);
}

void GameInstance::Tick(Seconds deltaTime)
{
	world->Tick(deltaTime);
}

void GameInstance::BeginPlay()
{

}

void GameInstance::EndPlay()
{

}

World* GameInstance::GetWorld() const
{
	return world.Get();
}