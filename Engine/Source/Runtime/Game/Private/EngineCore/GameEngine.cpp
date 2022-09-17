// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineCore/GameEngine.h"
#include "WorldCore/World.h"
#include "GameFramework/GameInstance.h"
#include "GameEngine.gen.cpp"

GameEngine::GameEngine() : Super()
{
}

void GameEngine::Init()
{
	Super::Init();
	GameWorld = SpawnGameWorld();
}

void GameEngine::Start(GameInstance* Instance)
{
	Super::Start(Instance);
	Instance->Start(GameWorld);
}

World* GameEngine::SpawnGameWorld()
{
	auto* Instance = gcnew World();
	Instance->Rename(this, TEXT("World"));
	return Instance;
}