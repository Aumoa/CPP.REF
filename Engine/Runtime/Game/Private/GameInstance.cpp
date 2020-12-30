// Copyright 2020 Aumoa.lib. All right reserved.

#include "GameInstance.h"

#include "World.h"

using namespace std;
using namespace std::chrono;

TRefPtr<String> GameInstance::defaultAppName = L"GameInstance";

GameInstance::GameInstance()
{

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
	world->LoadLevel(GetStartupLevel());
}

void GameInstance::Tick(duration<double> deltaTime)
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