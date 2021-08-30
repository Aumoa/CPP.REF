// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EngineSubsystems/GamePlayerSystem.h"
#include "GameFramework/LocalPlayer.h"
#include "GameEngine.h"

GamePlayerSystem::GamePlayerSystem() : Super()
{
}

GamePlayerSystem::~GamePlayerSystem()
{
}

void GamePlayerSystem::Init()
{
	Super::Init();
}

void GamePlayerSystem::Tick(std::chrono::duration<float> elapsedTime)
{
	Super::Tick(elapsedTime);
	_localPlayer->Tick(elapsedTime);
}

void GamePlayerSystem::SpawnLocalPlayer(IFrameworkView* frameworkView)
{
	_localPlayer = NewObject<LocalPlayer>();
	_localPlayer->Init(frameworkView);
}

LocalPlayer* GamePlayerSystem::GetLocalPlayer() const
{
	return _localPlayer;
}