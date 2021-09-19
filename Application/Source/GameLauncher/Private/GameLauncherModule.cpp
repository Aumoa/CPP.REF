// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameLauncherModule.h"
#include "GameLauncherInstance.h"

SGameInstance* SGameLauncherModule::CreatePrimaryGameModule()
{
	return NewObject<SGameLauncherInstance>();
}

DEFINE_GAME_MODULE(SGameLauncherModule);