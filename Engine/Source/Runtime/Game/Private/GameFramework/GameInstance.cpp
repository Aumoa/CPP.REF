// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameFramework/GameInstance.h"
#include "EngineCore/GameEngine.h"
#include "WorldCore/World.h"
#include "GameInstance.gen.cpp"

constexpr LogCategory LogGameInstance(TEXT("LogGameInstance"));

GameInstance::GameInstance()
{
}

void GameInstance::Start(World* Browser)
{
	Browser->BrowseLevel(StartupLevel);
}

GameEngine* GameInstance::GetEngine() noexcept
{
	return Cast<GameEngine>(GetOuter());
}

void GameInstance::Init()
{
	Log::Info(LogGameInstance, TEXT("GameInstance::Init() called."));
}

void GameInstance::Deinit()
{
	Log::Info(LogGameInstance, TEXT("GameInstance::Deinit() called."));
}