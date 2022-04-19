// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameInstanceSubsystem.h"
#include "GameInstance.h"

using namespace ::libty;

SGameInstanceSubsystem::SGameInstanceSubsystem() : Super()
{
}

void SGameInstanceSubsystem::Init(SGameInstance* GameInstance)
{
	this->GameInstance = GameInstance;
}

SGameInstance* SGameInstanceSubsystem::GetGameInstance()
{
	return GameInstance;
}