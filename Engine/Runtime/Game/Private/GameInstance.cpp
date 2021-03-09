// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameInstance.h"

#include "Engine.h"
#include "World.h"
#include "Framework/GameModeBase.h"
#include "Framework/PlayerController.h"
#include "Logging/LogMacros.h"
#include "Windows/CoreWindow.h"

using namespace std;
using namespace std::chrono;

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
	return AppName;
}

void GameInstance::Tick()
{
	engine->Tick();
}

void GameInstance::TickWorld(Seconds deltaTime)
{
	world->Tick(deltaTime);
}

Engine* GameInstance::GetEngine() const
{
	return engine.Get();
}

World* GameInstance::GetWorld() const
{
	return world.Get();
}

AGameModeBase* GameInstance::GetGameMode() const
{
	return gameMode;
}

APlayerController* GameInstance::GetLocalPlayer() const
{
	return localPlayerController;
}

TRefPtr<String> GameInstance::AppName_get() const
{
	return appName;
}

void GameInstance::AppName_set(TRefPtr<String> value)
{
	appName = value;
	if (MainWindow != nullptr)
	{
		MainWindow->Title = value;
	}
}

void GameInstance::Initialize()
{
	MainWindow->Title = appName;

	engine = NewObject<Engine>();
	engine->Initialize(this);

	world = NewObject<World>(this);
	gameMode = world->SpawnActor(GameModeClass);
	localPlayerController = world->SpawnActor(gameMode->PlayerControllerClass);
	if (localPlayerController == nullptr)
	{
		SE_LOG(LogEngine, Fatal, L"Cannot create player controller.");
		return;
	}
	world->RegisterPlayerController(localPlayerController);
	world->LoadLevel(gameMode->StartLevelClass);

	localPlayerController->BeginPlay();
}

void GameInstance::Shutdown()
{
	engine.Reset();
	world.Reset();
}