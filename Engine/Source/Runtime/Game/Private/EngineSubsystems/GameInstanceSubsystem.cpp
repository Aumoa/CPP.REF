// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameInstanceSubsystem.h"
#include "GameInstance.h"

SGameInstanceSubsystem::SGameInstanceSubsystem() : Super()
{
}

SGameInstance* SGameInstanceSubsystem::GetGameInstance()
{
	return Cast<SGameInstance>(GetOuter());
}