// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "THGameMode.h"

#include "Controller/THPlayerController.h"
#include "Levels/StartupLevel.h"

ATHGameMode::ATHGameMode()
{
	PlayerControllerClass = TSubclassOf<ATHPlayerController>::StaticClass();
	StartLevelClass = TSubclassOf<StartupLevel>::StaticClass();
}

ATHGameMode::~ATHGameMode()
{

}