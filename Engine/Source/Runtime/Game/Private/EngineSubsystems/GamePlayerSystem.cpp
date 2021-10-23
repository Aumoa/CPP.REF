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

void SGamePlayerSystem::SpawnLocalPlayer(IFrameworkView* frameworkView)
{
	_LocalPlayer = NewObject<SLocalPlayer>();
	_LocalPlayer->Init(frameworkView);
}

SLocalPlayer* SGamePlayerSystem::GetLocalPlayer() const
{
	return _LocalPlayer;
}