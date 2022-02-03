// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameInstanceSubsystem.h"
#include "GameInstance.h"

GENERATE_BODY(SGameInstanceSubsystem);

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