// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GamePlayerSystem.h"
#include "GameFramework/LocalPlayer.h"
#include "GameEngine.h"

SGamePlayerSystem::SGamePlayerSystem() : Super()
{
}

SGamePlayerSystem::~SGamePlayerSystem()
{
}

void SGamePlayerSystem::Tick(std::chrono::duration<float> elapsedTime)
{
	_localPlayer->Tick(elapsedTime);
}

void SGamePlayerSystem::SpawnLocalPlayer(IFrameworkView* frameworkView)
{
	_localPlayer = NewObject<SLocalPlayer>();
	_localPlayer->Init(frameworkView);
}

SLocalPlayer* SGamePlayerSystem::GetLocalPlayer() const
{
	return _localPlayer;
}